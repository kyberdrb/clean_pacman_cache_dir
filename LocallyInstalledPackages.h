//
// Created by laptop on 6/17/22.
//

#pragma once

#include "IgnoredPackageNames.h"
#include "LocallyInstalledPackage.h"

#include <set>
#include <memory>

class LocallyInstalledPackages {
public:
    explicit LocallyInstalledPackages(const IgnoredPackageNames& ignoredPackageNames);

    const Package& find(std::unique_ptr<Package>& packageWithInferredName) const;

    std::string generateReport() const;

private:
    const IgnoredPackageNames& ignoredPackageNames;

    // Assuming that each package has only one locally installed version with possibly multiple related package files for
    //  multiple versions that the Package will remember within itself
    //  and not as a value for the Package key.
    // "Some sets store object that embed their own keys, that is to say that such objects have a subpart that is to be considered as
    //  a key, like an ID for example, while the object itself is to be considered as a value."
    //   - https://www.fluentcpp.com/2017/06/09/search-set-another-type-key/
    //  Therefore a 'set' and
    //   - not a 'multiset' [the embedded key - package name - is unique - only one package name in all different versions of it
    //     than the local one],
    //   - not a 'map' [the values are related and contained in the key itself] and
    //   - not a 'multimap' [the key - package name - is unique - a filesystem feature: each file in a directory has a unique name]
    std::set<std::unique_ptr<Package>> locallyInstalledPackages;

    void fillLocallyInstalledPackages();
    void addLocallyInstalledPackage(std::unique_ptr<LocallyInstalledPackage> locallyInstalledPackage);
};
