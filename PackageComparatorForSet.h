// TODO delete this class - no longer used in the project

//
// Created by laptop on 5/7/22.
//

#pragma once

#include "Package.h"

// Create custom comparator as a functor in order to enable lookup ('find') in a 'set'
//  with elements as instances of a custom class i.e. with any custom object-type key
struct PackageComparatorForSet {
    bool operator() (const std::unique_ptr<Package>& lhs, const std::unique_ptr<Package>& rhs) const {
        return *lhs < *rhs;
    }
};
