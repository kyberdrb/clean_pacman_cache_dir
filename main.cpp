#include "IgnoredPackageNames.h"
#include "LocallyInstalledPackages.h"
#include "PackageNameMissing.h"
#include "PackageWithInferredName.h"
#include "SimpleInstallationPackageFile.h"

#include <filesystem>
#include <iostream>
#include <set>

//#include <cassert>

int main() {
    // FIND IGNORED PACKAGES PART - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES
    auto ignoredPackageNames = std::make_unique<IgnoredPackageNames>();

    // TODO replace with:
    //     TerminalPrinter::print(ignoredPackageNames->generateReport());
        std::cout << ignoredPackageNames->generateReport();

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES

    auto locallyInstalledPackages = std::make_unique<LocallyInstalledPackages>(*ignoredPackageNames);

    // FIND PACKAGE FILES IN DIFFERENT PACKAGE VERSIONS FOR SAME PACKAGE,
    //  PARTIALLY DOWNLOADED PACKAGE FILES AND
    //  PACKAGE FILES FOR MISSING PACKAGES

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;
    std::set<std::reference_wrapper<LocallyInstalledPackage>> packagesWithInstallationPackageFilesForDifferentVersions;

    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";
    std::filesystem::path pacmanCacheDirPath {pacmanCacheDir};

    for (const auto& packageFile : std::filesystem::directory_iterator(pacmanCacheDirPath)) {
        const auto& packageFileExtension = packageFile.path().extension().string();

        const auto& packageAbsolutePathAsText = packageFile.path().string();
        auto packageAbsolutePath = std::make_unique<AbsolutePath>(std::move( *(const_cast<std::string*>(&packageAbsolutePathAsText) ) ) );

        const auto& packageFilenameAsText = packageFile.path().filename().string();
        auto packageFilename = std::make_unique<Filename>(std::move( *(const_cast<std::string*>(&packageFilenameAsText) ) ) );

        if (packageFileExtension == ".part") {
            auto partlyDownloadedPackageFile= std::make_unique<SimpleInstallationPackageFile>(
                    std::move(packageAbsolutePath),
                    std::move(packageFilename),
                    SimpleInstallationPackageFileType::PARTIALLY_DOWNLOADED);

            partiallyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
            continue;
        }

        // For debugging purposes
//        if (packageFilenameAsText == "libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst") {
//            std::cout << "Here we go..." << "\n";
//        }

        if (packageFile.is_regular_file()) {
            std::string inferredPackageNameAsText = packageFilename->extractPackageNameAndVersion();

            std::unique_ptr<Package> packageWithInferredName =
                    std::make_unique<PackageWithInferredName>(std::move(inferredPackageNameAsText));

            auto* packageWithInferredNameExact = dynamic_cast<PackageWithInferredName*>(packageWithInferredName.get());

            // For debugging purposes
//            assert(packageWithInferredNameExact != nullptr);

            while ( packageWithInferredNameExact->hasStillSomethingInPackageName() ) {
                try {
                    // search for the matching package element in the 'locallyInstalledPackages' by 'packageWithInferredName'
                    const Package& matchingLocallyInstalledPackage = locallyInstalledPackages->find(packageWithInferredName);

                    // if the key WAS found,
                    //  - infer the package version from the compound package name and version,
                    //  - create a package file with filename, absolute path and package version
                    //     and add it to the matching locally installed package
                    //  - break out of the loop

                    // For debugging purposes
//                    assert(iteratorPointingToMatchingLocallyInstalledPackage.getName().string() == packageWithInferredName->getName().string());

                    auto inferredPackageVersion = packageWithInferredNameExact->extractPackageVersion();

                    auto packageRelatedFile = std::make_unique<ExtendedInstallationPackageFile>(
                            std::move(packageAbsolutePath),
                            std::move(packageFilename),
                            matchingLocallyInstalledPackage.getName(),
                            std::move(inferredPackageVersion));

                    const LocallyInstalledPackage& locallyInstalledPackageExact =
                            *( dynamic_cast<const LocallyInstalledPackage*>(&matchingLocallyInstalledPackage) );

                    // For debugging purposes
    //                assert(locallyInstalledPackageExact != nullptr);

                    LocallyInstalledPackage& localyInstalledPackageExactModifiable =
                            *( const_cast<LocallyInstalledPackage*>(&locallyInstalledPackageExact) );

                    bool wasInstallationPackageFileAdded = localyInstalledPackageExactModifiable.addPackageFileToDeletionCandidates(std::move(packageRelatedFile));

                    // if the package file was added to the deletion candidates for the particular package,
                    //  save the reference to the package file for generating only
                    //  and faster deleting of the package files by iterating only packages that have at least one package file for deletion
                    if (wasInstallationPackageFileAdded) {

                        packagesWithInstallationPackageFilesForDifferentVersions.emplace(
                                localyInstalledPackageExactModifiable
                        );
                    }

                    break;
                } catch (const PackageNameMissing& exception) {
                     // if key was NOT found, strip the compound package key from the end by one word up to the next delimiter
                     //  and perform lookup again

                     // For debugging purposes - because the gdb debugger in CLion 2022.1 produces an error when
                      // trying to show the values for STL containers and smartpointer instances.
                      // Instead, it shows an error message saying "Cannot instantiate printer for default visualizer"
//                    std::cout << exception.what() << "\n";

                    packageWithInferredNameExact->getNextInferredPackageNameCandidate();
                    continue;
                }
            }

            bool hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage = packageWithInferredNameExact->isPackageNameEmpty();

            if (hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage) {
                auto packageFileForMissingPackage = std::make_unique<SimpleInstallationPackageFile>(
                        std::move(packageAbsolutePath),
                        std::move(packageFilename),
                        SimpleInstallationPackageFileType::MISSING_LOCALLY_INSTALLED_PACKAGE);

                packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }

    std::cout << locallyInstalledPackages->generateReport();

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF INSTALLED PACKAGES THAT HAVE AT LEAST ONE RELATED INSTALLATION PACKAGE FILE FOR DIFFERENT VERSION THAN THE LOCALLY INSTALLED ONE\n\n";

    std::cout << "Found " << packagesWithInstallationPackageFilesForDifferentVersions.size()
        << " installed packages with installation package files for other than locally installed version\n\n";

    uint_fast16_t numberOfInstallationPackageFilesForOtherVersions = 0;

    for (const auto package : packagesWithInstallationPackageFilesForDifferentVersions) {
        std::cout << package << "\n";

        uint_fast16_t numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage =
            package.get().getNumberOfInstallationPackageFilesForDifferentVersions();

        numberOfInstallationPackageFilesForOtherVersions += numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage;
    }

    std::cout << "\n";
    std::cout << "Found " << numberOfInstallationPackageFilesForOtherVersions
        << " installation package files for different version than the locally installed\n";

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PARTIALLY DOWNLOADED INSTALLATION PACKAGE FILES\n\n";

    std::cout << "Found " << partiallyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : partiallyDownloadedPackageFiles) {
        std::cout << *partlyDownloadedPackageFile << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF INSTALLATION PACKAGE FILES RELATED TO MISSING PACKAGES\n\n";

    std::cout << "Found " << packageFilesRelatedToMissingPackages.size() << " package files related to missing packages\n\n";

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        std::cout << *packageFilesRelatedToMissingPackage << "\n";
    }

    // MOVING PACKAGE FILES TO SEPARATE DIRECTORY PART

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "MOVING PACKAGES\n\n";

    std::string pathToDuplicateFilesDirectoryAsText =
            pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";
    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    auto directoryForInstallationPackageFilesForDeletion = std::make_unique<AbsolutePath>(pathToDuplicateFilesDirectoryAsText);

    for (const auto& installedPackage : packagesWithInstallationPackageFilesForDifferentVersions) {
        installedPackage.get().movePackageFilesForDifferentVersionsToSeparateDir( *(directoryForInstallationPackageFilesForDeletion) );
    }

    for (const auto& partiallyDownloadedPackageFile : partiallyDownloadedPackageFiles) {
        partiallyDownloadedPackageFile->moveToSeparateDirectoryForDeletion(*(directoryForInstallationPackageFilesForDeletion));
    }

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        packageFilesRelatedToMissingPackage->moveToSeparateDirectoryForDeletion(*(directoryForInstallationPackageFilesForDeletion));
    }

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    // TODO completely clean all files ** only ** within the pikaur cache directory `"${HOME}/.cache/pikaur/"`

    // TODO completely clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/build/"`

    // TODO ** selectively ** clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/pkg/"

    return 0;
}
