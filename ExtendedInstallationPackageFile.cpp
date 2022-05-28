//
// Created by laptop on 4/21/22.
//

#include "ExtendedInstallationPackageFile.h"

ExtendedInstallationPackageFile::ExtendedInstallationPackageFile(
        std::unique_ptr<AbsolutePath> absolutePath,
        std::string filename,
        const PackageName& relatedPackageName,
        std::unique_ptr<PackageVersion> packageVersionOfPackageFile)
:
        absolutePath(std::move(absolutePath)),
        filename(std::move(filename)),
        relatedPackageName(relatedPackageName),
        relatedPackageVersion(std::move(packageVersionOfPackageFile))
{}

const std::string& ExtendedInstallationPackageFile::getFilename() const {
    return this->filename;
}

const AbsolutePath& ExtendedInstallationPackageFile::getAbsolutePath() const {
    return *(this->absolutePath);
}

const PackageName& ExtendedInstallationPackageFile::getRelatedPackageName() const {
    return this->relatedPackageName;
}

const PackageVersion& ExtendedInstallationPackageFile::getRelatedPackageVersion() const {
    return *(this->relatedPackageVersion);
}
