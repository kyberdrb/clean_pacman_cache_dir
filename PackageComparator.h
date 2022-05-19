//
// Created by laptop on 5/11/22.
//

#pragma once

#include "Package.h"

//struct PackageComparator {
//    bool operator()(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
//        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
////        return onePackage < anotherPackage;
//
//        // DEREFERENCED COMPARISON without accessor method - works with
//        //  - 'bool operator<(const Package& package) const'
//        //  - 'bool operator<(Package& package)'
//        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
////        return *onePackage < *anotherPackage;
//
//        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
//        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
//        //  Works out of the box for 'std::string'.
//        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
//        return onePackage->getName() < anotherPackage->getName();
//    }
//};

struct PackageComparator {
    bool operator()(const Package& onePackage, const std::unique_ptr<Package>& anotherPackage) const {
        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
//        return onePackage < anotherPackage;

        // DEREFERENCED COMPARISON without accessor method - works with
        //  - 'bool operator<(const Package& package) const'
        //  - 'bool operator<(Package& package)'
        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
//        return onePackage < *anotherPackage;

        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
        //  Works out of the box for 'std::string'.
        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
        return onePackage.getName() < anotherPackage->getName();
    }

    bool operator()(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) const {
        // DIRECT COMPARISON - works only with 'friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
//        return onePackage < anotherPackage;

        // DEREFERENCED COMPARISON without accessor method - works with
        //  - 'bool operator<(const Package& package) const'
        //  - 'bool operator<(Package& package)'
        //  - 'friend bool operator<(Package& onePackage, Package& anotherPackage)'
//        return *onePackage < anotherPackage;

        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'operator<' functions in this class ('Package') element to compared field within the 'Package' element
        //  which has 'operator<' implemented, leaving the 'Package' class intact. works without overloading 'std::less' in 'Package' class
        //  Works out of the box for 'std::string'.
        //  For custom type at least public friend 'operator<' with all const params needs to be implemented
        return onePackage->getName() < anotherPackage.getName();
    }
};
