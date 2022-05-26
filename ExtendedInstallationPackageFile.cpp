//
// Created by laptop on 4/21/22.
//

#include "ExtendedInstallationPackageFile.h"

ExtendedInstallationPackageFile::ExtendedInstallationPackageFile(std::string absolutePath) :
        filename(std::string{}),
        absolutePath(std::move(absolutePath)),
        relatedPackageName(*std::make_unique<PackageName>(std::string{}))  // I know it's a dirty and fragile solution - a hack - that
                                                                           // produces a dangling reference which - I promise - I won't use
                                                                           // but at least - with my current
                                                                           // knowledge - it's the simplest solution that compiles and runs
                                                                           // Possible solutions:
                                                                           //  - Maybe solve this with pointer to a 'Null Object' as 'Singleton'?
                                                                           //  - Maybe use inheritance? With abstract class? Pure virtual class/Interface?
                                                                           //  - Maybe use composition over inheritance?
                                                                           //     Conform to LSP - Liskov Substitution Principle -
                                                                           //     the 3rd principle of SOLID principles for software design?

{}

ExtendedInstallationPackageFile::ExtendedInstallationPackageFile(std::string filename, std::string absolutePath, const PackageName& relatedPackageName, std::unique_ptr<PackageVersion> packageVersionOfPackageFile) :
        filename(filename),
        absolutePath(absolutePath),
        relatedPackageName(relatedPackageName),
        packageVersionOfPackageFile(std::move(packageVersionOfPackageFile))
{}

std::string ExtendedInstallationPackageFile::getFilename() const {
    return this->filename;
}

std::string ExtendedInstallationPackageFile::getAbsolutePath() const {
    return this->absolutePath;
}

const PackageName& ExtendedInstallationPackageFile::getRelatedPackageName() const {
    return this->relatedPackageName;
}

const PackageVersion& ExtendedInstallationPackageFile::getRelatedPackageVersion() const {
    return *(this->packageVersionOfPackageFile);
}
