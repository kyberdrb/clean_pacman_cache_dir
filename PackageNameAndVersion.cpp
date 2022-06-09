//
// Created by laptop on 6/9/22.
//

#include "PackageNameAndVersion.h"

PackageNameAndVersion::PackageNameAndVersion(std::string packageNameAndVersionAsText) :
    nameAndVersion(std::move(packageNameAndVersionAsText))
{}

const std::string& PackageNameAndVersion::string() const {
    return PackageNameAndVersion::nameAndVersion;
}

std::string PackageNameAndVersion::substr(uint_fast8_t fromPosition) {
    return this->nameAndVersion.substr(fromPosition);
}
