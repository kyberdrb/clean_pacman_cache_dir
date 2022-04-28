//
// Created by laptop on 4/21/22.
//

#include "PackageFile.h"

PackageFile::PackageFile(std::string absolutePath) :
        absolutePath(std::move(absolutePath))
{}

//PackageFile::PackageFile(std::string filename, std::string absolutePath, std::string relatedPackageName, std::string relatedPackageVersion) :
//        filename(std::move(filename)),
//        absolutePath(std::move(absolutePath)),
//        relatedPackageName(std::make_unique<PackageName>(std::move(relatedPackageName) ) ),
//        relatedPackageVersion(std::make_unique<PackageVersion>(std::move(relatedPackageVersion) ) )
//{}

PackageFile::PackageFile(std::string filename, std::string absolutePath, std::unique_ptr<PackageWithInferredNameAndVersion> packageWithInferredNameAndVersion) :
        filename(std::move(filename)),
        absolutePath(std::move(absolutePath)),
        relatedPackage(std::move(packageWithInferredNameAndVersion) )
{}

std::string PackageFile::getFilename() const {
    return filename;
}

std::string PackageFile::getAbsolutePath() const {
    return absolutePath;
}

const PackageName& PackageFile::getRelatedPackageName() const {
    return this->relatedPackage->getName();
}

const PackageVersion& PackageFile::getRelatedPackageVersion() const {
    return this->relatedPackage->getVersion();
}

//const std::string& PackageFile::getRelatedPackageVersion() const {
//    return this->relatedPackageVersion->getVersion();
//}
