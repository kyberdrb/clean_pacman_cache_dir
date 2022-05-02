//
// Created by laptop on 5/2/22.
//

#pragma once

#include "IgnoredPackageNames.h"
#include "Package.h"

#include <set>
#include <ostream>

class LocallyInstalledPackages {
public:
    explicit LocallyInstalledPackages(const std::unique_ptr<IgnoredPackageNames>& ignoredPackageNames);
    //&LocallyInstalledPackages(const IgnoredPackageNames& ignoredPackageNames);

    std::string printInstalledPackages() const;

private:
    const std::unique_ptr<IgnoredPackageNames>& ignoredPackageNames;
    std::set<std::unique_ptr<Package>> installedPackages;

    void findLocallyInstalledPackages();

public:
    friend std::ostream& operator<<(std::ostream& os, const LocallyInstalledPackages& packages);
};
