//
// Created by laptop on 4/28/22.
//

#include "PackageWithInferredNameAndVersion.h"

PackageWithInferredNameAndVersion::PackageWithInferredNameAndVersion(std::unique_ptr<PackageName> inferredPackageName, std::string inferredPackageVersion) :
        inferredPackageName(std::move(inferredPackageName) ),
        inferredPackageVersion(std::make_unique<PackageVersion>(std::move(inferredPackageVersion) ) )
{}

const PackageName& PackageWithInferredNameAndVersion::getName() const {
    return *(this->inferredPackageName);
}

const PackageVersion& PackageWithInferredNameAndVersion::getVersion() const {
    return *(this->inferredPackageVersion);
}
