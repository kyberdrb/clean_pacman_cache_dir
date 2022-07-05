//
// Created by laptop on 6/8/22.
//

//
// Created by laptop on 4/9/22.
//

#include "PackageWithInferredName.h"

#include "FileMoverSingleton.h"

#include <iostream>

PackageWithInferredName::PackageWithInferredName(std::string extractedPackageNameAndVersionAsText) :
        Package(std::make_unique<PackageName>(std::move(extractedPackageNameAndVersionAsText) ) ),
        nameAndVersion(std::make_unique<PackageNameAndVersion>(Package::name->string() ) )
{}

bool PackageWithInferredName::isPackageNameEmpty() const {
    return this->name->empty();
}

bool PackageWithInferredName::hasStillSomethingInPackageName() const {
    return !(isPackageNameEmpty());
}

void PackageWithInferredName::getNextInferredPackageNameCandidate() {
    // When variale 'position' is of type 'auto' or 'unsigned' decimal type
    //  it causes 'out_of_range' exception when command '--position' underflows
    for (int32_t position = this->name->size() - 1; position >= 0; --position) {
        char delimiter = '-';
        bool weFoundDelimiterCharacter = this->name->at(position) == delimiter;
        this->name->pop_back();
        if (weFoundDelimiterCharacter) {
            break;
        }
    }
}

std::unique_ptr<PackageVersion> PackageWithInferredName::extractPackageVersion() {
    auto startingPositionForPackageVersion = this->getStartingPositionForPackageVersion();
    auto inferredPackageVersionAsText = this->nameAndVersion->substr(startingPositionForPackageVersion);
    return std::make_unique<PackageVersion>(std::move(inferredPackageVersionAsText));
}

uint_fast8_t PackageWithInferredName::getStartingPositionForPackageVersion() const {
    return this->name->size() + 1;
}