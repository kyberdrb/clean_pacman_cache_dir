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

const PackageVersion& Package::getLocallyInstalledVersion() const {
    return *(this->locallyInstalledVersion);
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

uint8_t Package::getStartingPositionForPackageVersion() const {
    return this->name->size() + 1;
}

uint_fast16_t Package::getNumberOfInstallationPackageFilesForDifferentVersions() const {
    return this->packageFilesForDeletion.size();
}

void Package::addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile) {
    bool isPackageNamesMatching =
            *(this->name) == packageRelatedPackageFile->getRelatedPackageName();

    bool isPackageVersionDifferent =
            *(this->locallyInstalledVersion) != packageRelatedPackageFile->getRelatedPackageVersion();

    if (isPackageVersionDifferent) {
        std::cerr << "this is what I was waiting for..." << "\n";
    }

    bool isPackageNonignored = !this->isIgnored;

    if ( isPackageNamesMatching && isPackageVersionDifferent && isPackageNonignored) {
        this->packageFilesForDeletion.emplace_back(std::move(packageRelatedPackageFile));
    }
}

void Package::movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles) {
    for (const auto& packageFileForDeletion : this->packageFilesForDeletion) {
        const std::string& from = packageFileForDeletion->getAbsolutePath();
        const std::string& to = pathToDirectoryForOtherVersionsOfPackageFiles +
                packageFileForDeletion->getFilename();
        std::cout << "Locally installed package:                    " <<
                  *(this->name) << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture << "\n";
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }
}
