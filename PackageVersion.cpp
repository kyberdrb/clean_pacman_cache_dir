//
// Created by laptop on 4/28/22.
//

//
// Created by laptop on 5/26/22.
//

#include "PackageVersion.h"

PackageVersion::PackageVersion(std::string packageVersionAsText) :
        version(std::move(packageVersionAsText))
{}

const std::string& PackageVersion::getVersionAsText() const {
    return PackageVersion::version;
}
