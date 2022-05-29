//
// Created by laptop on 4/9/22.
//

#include "Package.h"

#include <filesystem>
#include <iostream>

Package::Package(std::unique_ptr<PackageName> packageName, std::unique_ptr<PackageVersion> locallyInstalledVersion, std::string architecture, bool isIgnored) :
        name(std::move(packageName)),
        locallyInstalledVersion(std::move(locallyInstalledVersion)),
        architecture(std::move(architecture)),
        isIgnored(isIgnored)
{}

Package::Package(std::unique_ptr<PackageName> inferredPackageName) :
        name(std::move(inferredPackageName)),
        locallyInstalledVersion(std::make_unique<PackageVersion>(std::string{}) )
{}

const PackageName& Package::getName() const {
    return *(this->name);
}

bool Package::isPackageNameEmpty() const {
    return this->name->empty();
}

bool Package::hasStillSomethingInPackageName() const {
    return ! isPackageNameEmpty();
}

void Package::getNextInferredPackageNameCandidate() {
    for (int i = this->name->size() - 1; i >= 0; --i) {
        char delimiter = '-';
        bool weFoundDelimiterCharacter = this->name->at(i) == delimiter;
        this->name->pop_back();
        if (weFoundDelimiterCharacter) {
            break;
        }
    }
}

uint_fast8_t Package::getStartingPositionForPackageVersion() const {
    return this->name->size() + 1;
}

uint_fast16_t Package::getNumberOfInstallationPackageFilesForDifferentVersions() const {
    return this->installationPackageFilesForDifferentPackageVersions.size();
}

void Package::addPackageFileToDeletionCandidates(std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile) {
    bool isPackageNamesMatching =
            *(this->name) == packageRelatedPackageFile->getRelatedPackageName();

    bool isPackageVersionDifferent =
            *(this->locallyInstalledVersion) != packageRelatedPackageFile->getRelatedPackageVersion();

    // For debugging purposes
//    if (isPackageVersionDifferent) {
//        std::cerr << "this is what I was waiting for..." << "\n";
//    }

    bool isPackageNonignored = !this->isIgnored;

    if ( isPackageNamesMatching && isPackageVersionDifferent && isPackageNonignored) {
        this->installationPackageFilesForDifferentPackageVersions.emplace_back(std::move(packageRelatedPackageFile));
    }
}

void Package::movePackageFilesForDifferentVersionsToSeparateDir(
        const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles)
//void Package::movePackageFilesForDifferentVersionsToSeparateDir(
//        const std::string& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles)
//void Package::movePackageFilesForDifferentVersionsToSeparateDir()
{
    // TODO document leak: looks like there is a memory leak in 'std::filesystem' library
//    std::filesystem::rename("/var/cache/pacman/pkg/gtk4-1:4.6.4-1-x86_64.pkg.tar.zst", "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/gtk4-1:4.6.4-1-x86_64.pkg.tar.zst");
//
//    try {
//        std::filesystem::rename("/var/cache/pacman/pkg/gtk4-1:4.6.4-1-x86_64.pkg.tar.zst", "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/gtk4-1:4.6.4-1-x86_64.pkg.tar.zst");
//    } catch (const std::filesystem::__cxx11::filesystem_error& ex) {
//        std::cout << ex.what() << "\n";
//
//        std::cout << "Error: Insufficient permissions to move files." << "\n";
//        std::cout << "Please, run this program with elevated priviledges as 'sudo' or 'root' user." << "\n";
//        exit(1);
//    }

    for (const auto& packageFileForDeletion : this->installationPackageFilesForDifferentPackageVersions) {
        const std::string& from = packageFileForDeletion->getAbsolutePath().getAbsolutePath();

//        std::string absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles =
//                "/var/cache/pacman/pkg/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";

        const std::string& to = absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles + packageFileForDeletion->getFilename();
        std::cout << "Locally installed package:                    " <<
                  *(this->name) << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture << "\n";
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";

        try {
            std::filesystem::rename(from, to);
        } catch (const std::filesystem::__cxx11::filesystem_error& ex) {
            std::cout << ex.what() << "\n";

            std::cout << "Error: Insufficient permissions to move files." << "\n";
            std::cout << "Please, run this program with elevated priviledges as 'sudo' or 'root' user.\n---\n";
        }
    }
}
