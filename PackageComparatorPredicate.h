//
// Created by laptop on 5/12/22.
//

#pragma once

#include "Package.h"

// using Predicate instead (reusable; no modification of the compared class)

// FOR PASSING SMART POINTER TO COMPARATOR PREDICATE DIRECTLY

//struct PackageComparatorPredicate {
//    const std::unique_ptr<Package>& package;
//
//    explicit PackageComparatorPredicate(const std::unique_ptr<Package>& packageToFind) :
//            package(packageToFind)
//    {}
//
//    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
//        // DIRECT COMPARISON - works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)'
////        return (this->package == otherPackage);
//
//        // DEREFERENCED COMPARISON without accessor method - works with
//        //  - 'friend bool operator==(Package& onePackage, Package& anotherPackage)' (with/without const for parameters)
//        //  - and member function 'bool operator==(Package& otherPackage)' (with/without const for function/parameters)
////        return (*(this->package) == *otherPackage);
//
//        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'Package' element to compared field within the 'Package' element
//        return (this->package->getName() == otherPackage->getName());
//    }
//};

// FOR PASSING DEREFERENCED SMART POINTER TO COMPARATOR PREDICATE

struct PackageComparatorPredicate {
    const Package& package;

    explicit PackageComparatorPredicate(const Package& packageToFind) :
            package(packageToFind)
    {}

    bool operator()(const std::unique_ptr<Package>& otherPackage) const {
        // COMPARISON OF SEARCHED DEREFERENCED MEMBER VARIABLE WITH SMART POINTER
        //  - works with 'friend bool operator==(Package& onePackage, std::unique_ptr<Package>& anotherPackage)'
//        return this->package == otherPackage;

        // COMPARISON OF MEMBER VARIABLE REFERENCE WITH DEREFERENCED SMART POINTER
        //  - 'friend bool operator==(Package& onePackage, Package& anotherPackage)' (with/without const for parameters)
        //  - and member function 'bool operator==(Package& otherPackage)' (with/without const for function/parameters)
        return (this->package == *otherPackage);

        // DEREFERENCED COMPARISON with accessor method - delegating comparison from 'Package' element to compared field within the 'Package' element
//        return (this->package.getName() == otherPackage->getName());
    }
};
