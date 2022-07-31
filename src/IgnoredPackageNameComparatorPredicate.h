//
// Created by laptop on 6/17/22.
//

#pragma once

#include "IgnoredPackageName.h"

class IgnoredPackageNameComparatorPredicate {
public:
    explicit IgnoredPackageNameComparatorPredicate(const std::unique_ptr<IgnoredPackageName>& ignoredPackageNameCandidate) :
            ignoredPackageNameCandidate(ignoredPackageNameCandidate)
    {}

    bool operator()(const std::unique_ptr<IgnoredPackageName>& otherPackage) const {
        // delegate comparison to 'operator==' with all parameters of type 'unique_ptr<IgnoredPackageName>&' in 'IgnoredPackageName' class
        return (this->ignoredPackageNameCandidate == otherPackage);
    }

private:
    const std::unique_ptr<IgnoredPackageName>& ignoredPackageNameCandidate;
};
