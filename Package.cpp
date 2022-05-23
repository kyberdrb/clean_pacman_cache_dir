//
// Created by laptop on 4/9/22.
//

#include "Package.h"

#include <filesystem>
#include <iostream>

#include <cassert>

Package::Package(std::string name, std::unique_ptr<PackageName> packageName, std::string locallyInstalledVersion, std::string architecture, bool isIgnored) :
        nameAsText(std::move(name)),
        name(std::move(packageName)),
        locallyInstalledVersion(std::move(locallyInstalledVersion)),
        architecture(std::move(architecture)),
        isIgnored(isIgnored)
{
    assert(this->nameAsText == this->name->string());
}

Package::Package(std::string inferredPackageNameAsText, std::unique_ptr<PackageName> inferredPackageName) :
        nameAsText(std::move(inferredPackageNameAsText)),
        name(std::move(inferredPackageName))
{}

std::string Package::getName() const {
    return nameAsText;
}

std::unique_ptr<PackageName>& Package::getNameAsInstance() {
    return this->name;
}

bool Package::isPackageNameEmpty() const {
    return this->nameAsText.empty();
}

bool Package::hasStillSomethingInPackageName() const {
    return ! isPackageNameEmpty();
}

void Package::getNextInferredPackageNameCandidate() {
    for (int i = this->nameAsText.size() - 1; i >= 0; --i) {
        char delimiter = '-';
        bool weFoundDelimiterCharacter = this->nameAsText.at(i) == delimiter;
        this->nameAsText.pop_back();
        this->name->pop_back();
        if (weFoundDelimiterCharacter) {
            break;
        }
    }
}

uint8_t Package::getStartingPositionForPackageVersion() const {
    return this->nameAsText.size() + 1;
}

uint_fast16_t Package::getNumberOfInstallationPackageFilesForDifferentVersions() const {
    return this->packageFilesForDeletion.size();
}

void Package::addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile) {
    bool isPackageNamesMatching = this->nameAsText == packageRelatedPackageFile->getRelatedPackageName();
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
                  this->nameAsText << "-" << this->locallyInstalledVersion << "-" << this->architecture << "\n";
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }
}
