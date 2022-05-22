//
// Created by laptop on 4/9/22.
//

#include "Package.h"

#include <filesystem>
#include <iostream>

Package::Package(std::string name, std::string locallyInstalledVersion, std::string architecture, bool isIgnored) :
        name(std::move(name)),
        locallyInstalledVersion(std::move(locallyInstalledVersion)),
        architecture(std::move(architecture)),
        isIgnored(isIgnored)
{}

Package::Package(std::string inferredPackageName) :
    name(inferredPackageName)
{}

std::string Package::getName() const {
    return name;
}

bool Package::isPackageNameEmpty() const {
    return this->name.empty();
}

bool Package::hasStillSomethingInPackageName() const {
    return ! isPackageNameEmpty();
}

void Package::getNextInferredPackageNameCandidate() {
    for (int i = this->name.size() - 1; i >= 0;             --i) {
        char delimiter = '-';
        bool weFoundDelimiterCharacter = this->name.at(i) == delimiter;
        this->name.pop_back();
        if (weFoundDelimiterCharacter) {
            break;
        }
    }
}

uint8_t Package::getStartingPositionForPackageVersion() const {
    return this->name.size() + 1;
}

bool Package::hasInstallationPackageFilesForDifferentVersions() const {
    return this->packageFilesForDeletion.size() >= 1;
}

void Package::addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile) {
    bool isPackageNamesMatching = this->name == packageRelatedPackageFile->getRelatedPackageName();
    bool isPackageVersionDifferent = this->locallyInstalledVersion != packageRelatedPackageFile->getRelatedPackageVersion();
    bool isPackageNonignored = !this->isIgnored;

    if ( isPackageNamesMatching && isPackageVersionDifferent && isPackageNonignored) {
        this->packageFilesForDeletion.emplace_back(std::move(packageRelatedPackageFile));
    }
}

void Package::movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles) {
    for (const auto& packageFileForDeletion: this->packageFilesForDeletion) {
        const std::string& from = packageFileForDeletion->getAbsolutePath();
        const std::string& to = pathToDirectoryForOtherVersionsOfPackageFiles +
                packageFileForDeletion->getFilename();
        std::cout << "Locally installed package:                    " <<
            this->name << "-" << this->locallyInstalledVersion << "-" << this->architecture << "\n";
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }
}
