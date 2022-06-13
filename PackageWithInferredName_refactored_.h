//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package_refactored_.h"
#include "PackageVersion.h"
#include "PackageNameAndVersion.h"

#include <iostream>

class PackageWithInferredName_refactored_ : public Package_refactored_ {
public:
    explicit PackageWithInferredName_refactored_(std::string extractedPackageNameAndVersionAsText);

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    std::unique_ptr<PackageVersion> extractPackageVersion();

protected:
    std::ostream& streamOutputOperator(std::ostream& out) const override {
        out
            << *(this->nameAndVersion) << "\t"
            << Package_refactored_::getName();

        return out;
    }

    bool lessThanOperator(const std::unique_ptr<Package_refactored_>& anotherPackage) const override {
        return Package_refactored_::getName() < anotherPackage->getName();
    }

    bool lessThanOperator(const std::reference_wrapper<Package_refactored_>& anotherPackage) override {
        return Package_refactored_::getName() < anotherPackage.get().getName();
    }

private:
    std::unique_ptr<PackageNameAndVersion> nameAndVersion;

    uint_fast8_t getStartingPositionForPackageVersion() const;
};