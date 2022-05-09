//
// Created by laptop on 5/8/22.
//

#pragma once

#include "IgnoredPackageName.h"

// Comparator for std::map with unique_ptr keys didn't work for std::vector with unique_ptr elements...
//struct IgnoredPackageNamesEqualityComparator {
//    bool operator()(const PackageName& firstPackageName, const PackageName& secondPackageName) const {
//        return firstPackageName == secondPackageName;
//    }
//};

// ...using Predicate (pre C++11 solution that didn't support lambda expressions)
struct IgnoredPackageNamesEqualityComparator {
    explicit IgnoredPackageNamesEqualityComparator(const std::unique_ptr<IgnoredPackageName>& packageName) :
            ignoredPackageName(packageName)
    {}

    bool operator()(const std::unique_ptr<IgnoredPackageName>& otherPackageName) const {
//        return (ignoredPackageName == otherPackageName);

//        return *ignoredPackageName == *otherPackageName;
//        return *(this->ignoredPackageName) == *otherPackageName;
        return *(IgnoredPackageNamesEqualityComparator::ignoredPackageName) == *otherPackageName;
    }

private:
    const std::unique_ptr<IgnoredPackageName>& ignoredPackageName;
};
