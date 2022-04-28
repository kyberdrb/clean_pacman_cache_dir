//
// Created by laptop on 4/28/22.
//

#pragma once

#include "PackageName.h"
#include "PackageVersion.h"

#include <string>

class PackageWithInferredNameAndVersion {
public:
    PackageWithInferredNameAndVersion(std::string inferredPackageName, std::string inferredPackageVersion);

    const PackageName& getName() const;
    const PackageVersion& getVersion() const;

private:
    std::unique_ptr<PackageName> inferredPackageName;
    std::unique_ptr<PackageVersion> inferredPackageVersion;
};
