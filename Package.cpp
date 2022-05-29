//
// Created by laptop on 4/9/22.
//

#include "Package.h"
#include "FileMover.h"

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
{
    for (const auto& packageFileForDeletion : this->installationPackageFilesForDifferentPackageVersions) {
        const AbsolutePath& from = packageFileForDeletion->getAbsolutePath();
        const auto to = std::make_unique<AbsolutePath>(
                absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles + packageFileForDeletion->getFilename());

        std::cout << "Locally installed package:\t" <<
                  *(this->name) << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture << "\n";
        std::cout << "Moving package file\t\t" << from << "\n";
        std::cout << "to separate directory\t\t" << *(to) << "\n\n";

        FileMover::move(from, *(to));
    }
}
