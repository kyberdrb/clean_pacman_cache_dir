//
// Created by laptop on 4/28/22.
//

#include "PackageVersion.h"

PackageVersion::PackageVersion(std::string packageVersionAsText) :
    version(std::move(packageVersionAsText))
{}

bool PackageVersion::empty() const {
    return this->version.empty();
}

const char& PackageVersion::at(size_t position) {
    return this->version.at(position);
}

const std::string& PackageVersion::getVersion() const {
    return version;
}
