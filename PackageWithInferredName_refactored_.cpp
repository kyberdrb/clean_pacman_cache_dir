//
// Created by laptop on 6/8/22.
//

//
// Created by laptop on 4/9/22.
//

#include "PackageWithInferredName_refactored_.h"

#include "FileMover.h"

#include <filesystem>
#include <iostream>

PackageWithInferredName_refactored_::PackageWithInferredName_refactored_(std::unique_ptr<PackageNameAndVersion> inferredPackageNameAndVersion) :
        nameAndVersion(std::move(inferredPackageNameAndVersion) ),
        name(std::make_unique<PackageName>(this->nameAndVersion->string() ) ),
        locallyInstalledVersion(std::make_unique<PackageVersion>(std::string{}) )
{}

const PackageName& PackageWithInferredName_refactored_::getName() const {
    return *(this->name);
}

bool PackageWithInferredName_refactored_::isPackageNameEmpty() const {
    return this->name->empty();
}

bool PackageWithInferredName_refactored_::hasStillSomethingInPackageName() const {
    return ! isPackageNameEmpty();
}

void PackageWithInferredName_refactored_::getNextInferredPackageNameCandidate() {
    for (int i = this->name->size() - 1; i >= 0; --i) {
        char delimiter = '-';
        bool weFoundDelimiterCharacter = this->name->at(i) == delimiter;
        this->name->pop_back();
        if (weFoundDelimiterCharacter) {
            break;
        }
    }
}

std::unique_ptr<PackageVersion> PackageWithInferredName_refactored_::extractPackageVersion() {
    auto startingPositionForPackageVersion = this->getStartingPositionForPackageVersion();
    auto inferredPackageVersionAsText = this->nameAndVersion->substr(startingPositionForPackageVersion);
    return std::make_unique<PackageVersion>(std::move(inferredPackageVersionAsText));
}

uint_fast8_t PackageWithInferredName_refactored_::getStartingPositionForPackageVersion() const {
    return this->name->size() + 1;
}