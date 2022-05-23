//
// Created by laptop on 5/23/22.
//

#pragma once

#include "Package.h"

struct PackageComparator {
    bool operator() (const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) const {
        // WORKS standalone
//        return package->getName() < anotherPackage->getName();

        // WORKS standalone
        return package->getNameAsInstance()->string() < anotherPackage->getNameAsInstance()->string();


        // WORKS with overloaded 'operator<' in class 'PackageName' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)'
//        return package->getNameAsInstance() < anotherPackage->getNameAsInstance();

        // WORKS with overloaded 'operator<' in class 'PackageName' defined as 'friend bool operator<(const PackageName& packageName, const PackageName& otherPackageName)'
//        return *package->getNameAsInstance() < *anotherPackage->getNameAsInstance();
    }
};
