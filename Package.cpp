//
// Created by laptop on 4/9/22.
//

#include <utility>
#include <vector>
#include "Package.h"

Package::Package(std::string name, std::string locallyInstalledVersion, std::string architecture) :
        name(std::move(name)),
        locallyInstalledVersion(std::move(locallyInstalledVersion)),
        architecture(std::move(architecture))
{}

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
