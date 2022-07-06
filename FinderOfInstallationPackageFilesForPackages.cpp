//
// Created by laptop on 5/3/22.
//

#include "FinderOfInstallationPackageFilesForPackages.h"

#include <algorithm>
#include <filesystem>

// For debugging purposes, because the debugger in CLion 2022.1 is broken
//#include <iostream>

FinderOfInstallationPackageFilesForPackages::FinderOfInstallationPackageFilesForPackages(
    std::unique_ptr<LocallyInstalledPackages>& locallyInstalledPackages) :
        locallyInstalledPackages(locallyInstalledPackages)
{
    this->relateInstallationPackageFilesToLocallyInstalledPackages();
}

void FinderOfInstallationPackageFilesForPackages::relateInstallationPackageFilesToLocallyInstalledPackages() {
    std::filesystem::path pacmanCacheDir {FinderOfInstallationPackageFilesForPackages::PACMAN_CACHE_DIR_PATH};

    for (const auto& packageFile : std::filesystem::directory_iterator(pacmanCacheDir)) {
        const auto& packageFilenameAsText = packageFile.path().filename().string();
        const auto& packageAbsolutePathAsText = packageFile.path().string();
        const auto& packageFileExtension = packageFile.path().extension().string();

        if (packageFileExtension == ".part") {
            auto partlyDownloadedPackageFile= std::make_unique<PackageFile>(packageAbsolutePathAsText);
            this->partlyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
            continue;
        }

        // For debugging purposes
//        if (packageFilenameAsText == "libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst") {
//            std::cout << "Here we go..." << "\n";
//        }

        if (packageFile.is_regular_file()) {
            // strip the extensions and architecture from package filename leaving only package name and package version
            //  I couldn't find out how to do first-match or non-greedy replace with regex_replace,
            //  so I'm writing my own algorithm
            auto packageFilenameAsTextCopy = packageFilenameAsText;
            std::reverse(packageFilenameAsTextCopy.begin(), packageFilenameAsTextCopy.end());
            std::stringstream packageNameAndVersionReversed{};
            char delimiter = '-';
            bool stillSearchingForFirstDelimiterOccurence = true;

            //find the first dash '-' in the reversed filename and append everything after
            for (auto character : packageFilenameAsTextCopy) {
                if (character != delimiter && stillSearchingForFirstDelimiterOccurence) {
                    continue;
                }

                stillSearchingForFirstDelimiterOccurence = false;
                packageNameAndVersionReversed << character;
            }

            auto packageNameAndVersion = packageNameAndVersionReversed.str();
            std::reverse(packageNameAndVersion.begin(), packageNameAndVersion.end());
            packageNameAndVersion.pop_back();

            std::string inferredPackageNameAsText = packageNameAndVersion;
            auto packageWithInferredName = std::make_unique<Package>(std::move(inferredPackageNameAsText));
            bool wasInferredPackageRefferingToMissingPackage = false;

            // TODO encapsulate package search by its inferred name into a bool function in  LocallyInstalledPackages class
            //  and assign the result to the variable 'wasInferredPackageRefferingToMissingPackage'
            while ( packageWithInferredName->hasStillSomethingInPackageName() ) {
                // For debugging purposes (because the debugger in CLion 2022.1 instead of values for STL container
                //  elements and class member fields shows an error "Cannot instantiate printer for default visualizer")
                //  and in the 'Debug -> Debugger -> GDB' tabs show an error in '/opt/clion/bin/gdb/renderers/libstdcxx/v6/printers.py'
                //  Going to use an external GDB instead. But which one?
//                std::cout << *packageWithInferredName << "\n";

                // search for the matching package element in the 'locallyInstalledPackages' by 'packageWithInferredName'
                auto matchingPackage = this->locallyInstalledPackages->find(packageWithInferredName);

                // TODO encapsulate matchingPackage comparison in a public function in the LocallyInstalledPackages class
                // if key was NOT found, generate a new key candidate and perform lookup again
                bool packageWithInferredNameIsMissing = matchingPackage == this->locallyInstalledPackages->end();
                if (packageWithInferredNameIsMissing) {
                    packageWithInferredName->getNextInferredPackageNameCandidate();
                    continue;
                }

                // if the key WAS found,
                //  - infer the package version from the compound package name and version,
                //  - create a package file with filename, absolute path and package version and add it to the matching locally installed package
                //  - break out of the loop
                auto startingPositionForPackageVersion = packageWithInferredName->getStartingPositionForPackageVersion();
                auto inferredPackageVersionAsText = packageNameAndVersion.substr(startingPositionForPackageVersion);
                wasInferredPackageRefferingToMissingPackage = inferredPackageVersionAsText.empty();

                std::unique_ptr<PackageName> inferredPackageName = std::move(packageWithInferredName->moveNameHere());

                auto packageWithInferredNameAndVersion = std::make_unique<PackageWithInferredNameAndVersion>(
                        std::move(inferredPackageName),
                        std::move(inferredPackageVersionAsText));

                auto packageRelatedFile = std::make_unique<PackageFile>(
                        packageFilenameAsText,
                        packageAbsolutePathAsText,
                        std::move(packageWithInferredNameAndVersion) );

                matchingPackage->get()->addPackageFileToDeletionCandidates(std::move(packageRelatedFile));
                break;
            }

            // If the lookup didn't find matching package name for all of the package filename candidates
            //  the package file is likely to be referring to a missing or uninstalled package
            if (wasInferredPackageRefferingToMissingPackage) {
                auto packageFileForMissingPackage = std::make_unique<PackageFile>(packageAbsolutePathAsText);
                this->packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }
}

std::string FinderOfInstallationPackageFilesForPackages::generateReport() const {
    std::stringstream textRepresentationOfInstanceAsStream{};
    textRepresentationOfInstanceAsStream << *this;
    return textRepresentationOfInstanceAsStream.str();
}

std::ostream& operator<<(std::ostream& out, const FinderOfInstallationPackageFilesForPackages& packages) {
    out << *(packages.locallyInstalledPackages);

    // TODO encapsulate to separate class 'PartlyDownloadedPackageFiles'
    out << "\n";
    out << "===============================================\n\n";
    out << "LIST OF PARTLY DOWNLOADED PACKAGE FILES\n\n";

    out << "Found " << packages.partlyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : packages.partlyDownloadedPackageFiles) {
        out << *partlyDownloadedPackageFile << "\n";
    }

    // TODO encapsulate to separate class 'PackageFilesRelatedToMissingPackages'
    out << "\n";
    out << "===============================================\n\n";
    out << "LIST OF PACKAGE FILES RELATED TO MISSING PACKAGES\n\n";

    out << "Found " << packages.packageFilesRelatedToMissingPackages.size() << " package files related to missing packages\n\n";

    for (const auto& packageFilesRelatedToMissingPackage : packages.packageFilesRelatedToMissingPackages) {
        out << *packageFilesRelatedToMissingPackage << "\n";
    }

    return out;
}
