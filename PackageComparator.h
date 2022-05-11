//
// Created by laptop on 5/11/22.
//

#pragma once

#include "Package.h"

struct PackageComparator {
// DIRECT COMPARISON
//    bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
//        return onePackage < anotherPackage;
//    }

// DEREFERENCED COMPARISON without accessor method
//    bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
//        return *onePackage < *anotherPackage;
//    }

// DEREFERENCED COMPARISON with accessor method - delegating comparison from 'Package' element to compared field in 'Package' element
//  which has 'operator<' implemented, leaving the 'Package' class intact and without overloading 'std::less' in 'Package' class
    bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
        return onePackage->getName() < anotherPackage->getName();
    }
};
