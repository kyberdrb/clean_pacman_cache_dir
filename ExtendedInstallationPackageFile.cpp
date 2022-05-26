//
// Created by laptop on 4/21/22.
//

#include "ExtendedInstallationPackageFile.h"

// TODO swap position of parameters 'filename' and 'absolutePath'
ExtendedInstallationPackageFile::ExtendedInstallationPackageFile(std::string absolutePath, std::string filename, const PackageName& relatedPackageName, std::unique_ptr<PackageVersion> packageVersionOfPackageFile) :
        absolutePath(std::move(absolutePath)),
        filename(std::move(filename)),
        relatedPackageName(relatedPackageName),
        packageVersionOfPackageFile(std::move(packageVersionOfPackageFile))
{}

const std::string& ExtendedInstallationPackageFile::getFilename() const {
    return this->filename;
}

const std::string& ExtendedInstallationPackageFile::getAbsolutePath() const {
    return this->absolutePath;
}

const PackageName& ExtendedInstallationPackageFile::getRelatedPackageName() const {
    return this->relatedPackageName;
}

const PackageVersion& ExtendedInstallationPackageFile::getRelatedPackageVersion() const {
    return *(this->packageVersionOfPackageFile);
}
