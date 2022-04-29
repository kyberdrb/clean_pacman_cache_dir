//
// Created by laptop on 4/29/22.
//

#pragma once

#include "PackageName.h"

// Comparator for std::map with unique_ptr keys didn't work for std::vector with unique_ptr elements...
//struct PackageNamesEqualityComparator {
//    bool operator()(const PackageName& firstPackageName, const PackageName& secondPackageName) const {
//        return firstPackageName == secondPackageName;
//    }
//};

// ...using Predicate instead (pre C++11 solution)
struct PackageNamesEqualityComparator {
    const std::unique_ptr<PackageName>& firstPackageName;

    explicit PackageNamesEqualityComparator(const std::unique_ptr<PackageName>& packageName) :
            firstPackageName(packageName)
    {}

    bool operator()(const std::unique_ptr<PackageName>& otherPackageName) const {
        return (*firstPackageName == *otherPackageName);
    }
};