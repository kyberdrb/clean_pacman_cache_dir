//
// Created by laptop on 4/21/22.
//

#include "PackageFile.h"

PackageFile::PackageFile(std::string absolutePath) :
        absolutePath(std::move(absolutePath)),
        relatedPackageName(*std::make_unique<PackageName>(std::string{})) // I know it's a dirty and fragile solution - a hack - that
                                                                          // produces a dangling reference which - I promise - I won't use
                                                                          // but at least - with my current
                                                                          // knowledge - it's the simplest solution that compiles and runs
{}

PackageFile::PackageFile(std::string filename, std::string absolutePath, const PackageName& relatedPackageName, std::string relatedPackageVersion) :
        filename(filename),
        absolutePath(absolutePath),
        relatedPackageName(relatedPackageName),
        relatedPackageVersion(relatedPackageVersion)
{}

std::string PackageFile::getFilename() const {
    return filename;
}

std::string PackageFile::getAbsolutePath() const {
    return absolutePath;
}

const PackageName& PackageFile::getRelatedPackageName() const {
    return this->relatedPackageName;
}

std::string PackageFile::getRelatedPackageVersion() const {
    return this->relatedPackageVersion;
}
