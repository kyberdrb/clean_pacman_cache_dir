//
// Created by laptop on 4/9/22.
//

#include "Package.h"

#include <iostream>
#include <utility>
#include <vector>

Package::Package(std::string name, std::string locallyInstalledVersion, std::string architecture) :
        name(std::move(name)),
        locallyInstalledVersion(std::move(locallyInstalledVersion)),
        architecture(std::move(architecture))
{}

Package::Package(std::string inferredPackageName) :
    name(inferredPackageName)
{}

bool Package::addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile) {
    if (
            this->name != packageRelatedPackageFile->getRelatedPackageName() &&
            this->locallyInstalledVersion != packageRelatedPackageFile->getRelatedPackageVersion())
    {
        this->packageFilesForDeletion.emplace_back(std::move(packageRelatedPackageFile));
        return true;
    }

    return false;
}

std::vector<std::string> Package::getPackageNameCandidates() const {
    // Naming convention for packages
    //  (lib32-)?<package_name>-<package_version>-<architecture>.pkg.tar.[zst|xz](.sig)?

    std::vector<std::string> prefixes{"", "lib32-"};
    std::vector<std::string> packageNameCandidatesWithPrefixes{};

    for (const auto& prefix : prefixes) {
        std::string compoundPackageNameWithPrefix = prefix + this->name + this->locallyInstalledVersion + this->architecture + ".pkg.tar";
        packageNameCandidatesWithPrefixes.emplace_back(std::move(compoundPackageNameWithPrefix));
    }

    std::vector<std::string> compoundPackageNames{};
    std::vector<std::string> suffixesOrExtensions{"zst", "xz", "sig"};  // TODO change for a weak_ptr on Extensions instance

    for (const auto& packageNameWithPrefix : packageNameCandidatesWithPrefixes) {
        for (const auto &extension: suffixesOrExtensions) {
            std::string completePackageName = packageNameWithPrefix + extension;
            compoundPackageNames.emplace_back(std::move(completePackageName));
        }
    }

    return std::move(compoundPackageNames);
}

std::string Package::getLocallyInstalledVersion() const {
    return locallyInstalledVersion;
}

bool Package::isSpecial() const {
    // TODO handle special package names:
    //   if the first character packageName is a number
    //     create a <PackageName, Package> pair and add it to the specialPackages map [packageName will be empty - all will be saved in the packageVersion]
    //  For multi-word package names:
    //   tokenize the packageName by dashes
    //   for each token in tokens
    //     when the first character of the token is a number
    //       create a <PackageName, Package> pair and add it to the specialPackages map [packageName will be incomplete - the rest of the package filename will be save in the packageVersion]

    // TODO handle special package versions:
    //   if the first character packageVersion is a letter
    //     create a <PackageName, Package> pair and add it to the specialPackages map [packageVersion may be empty or incomplete - the rest will be saved in the packageName]
    //  For multi-word package versions (e.g. with included release version):
    //   tokenize the packageVersion by dashes
    //   for each token in tokens
    //     when the first character of the token is a letter
    //       create a <PackageName, Package> pair and add it to the specialPackages map [packageName may be empty or incomplete - the rest of the package version will be save in the packageName]
    //  Note for handling special packages: in (rare) cases there occure package names with numbers as the first character e.g. '0ad' and  after the dash e.g. '-1.16'
    //   and package version that begin with a letter e.g. '	a25.b-3'
    //    - adjust 'operator<'?
    //    - write a custom comparator that will check character by character for prefix similarity?
    //    - add them to ignoredPackages?
    //    - add them to unhandled/other/special packages and then handle manually?/automatically? << MAYBE THIS WILL BE THE OPTION I'll try out
    //    - assemble a package filename by trial-and-error from available extensions and check whether the file under assembles filename exists?
    //  Problematic package filenames use-cases:
    //   libyuv-r2212+dfaf7534-2-x86_64.pkg.tar.zst
    //   libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst
    //   libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst.sig
    //  the parsing algorithm breaks when the first character of any of the tokens belonging to the package filename is a number
    //   - then packageName stays empty or incomplete, the packageVersion has the rest of the package filename together with the version
    //   or when the package version has as the first token a letter

    if ( ( std::isdigit(this->name.at(0) ) ) || ( ! std::isdigit(this->locallyInstalledVersion.at(0) ) ) ) {
        return true;
    }

    return false;
}

std::string Package::buildPartialPackageNamePrefix() const {
    // Verify whether the name and locallyInstalledVersion will be enough to find a single related package
    return this->name + "-" + this->locallyInstalledVersion + "-" + this->architecture;
}

std::string Package::getName() const {
    return name;
}

bool Package::isEmpty() const {
    return this->name.empty() && this->locallyInstalledVersion.empty() && this->architecture.empty();
}

void Package::movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles) {
    for (const auto& packageFileForDeletion: this->packageFilesForDeletion) {
        const std::string& from = packageFileForDeletion->getAbsolutePath();
        const std::string& to = pathToDirectoryForOtherVersionsOfPackageFiles + packageFileForDeletion->getFilename();
        std::cout << "Moving package file\t" << from << "\nto separate directory\t" << to << "\n\n";
//            std::filesystem::rename(from, to);
    }
}
