//
// Created by laptop on 4/21/22.
//

#include "PackageFile.h"

PackageFile::PackageFile(std::string absolutePath) :
        absolutePath(std::move(absolutePath))
{}

PackageFile::PackageFile(std::string filename, std::string absolutePath, std::string relatedPackageName, std::string relatedPackageVersion) :
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

std::string PackageFile::getRelatedPackageName() const {
    return this->relatedPackageName;
}

std::string PackageFile::getRelatedPackageVersion() const {
    return this->relatedPackageVersion;
}
