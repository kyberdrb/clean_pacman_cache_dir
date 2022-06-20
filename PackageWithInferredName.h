//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package.h"
#include "PackageVersion.h"
#include "PackageNameAndVersion.h"

#include <iostream>

class PackageWithInferredName : public Package {
public:
    explicit PackageWithInferredName(std::string extractedPackageNameAndVersionAsText);

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    std::unique_ptr<PackageVersion> extractPackageVersion();

protected:
    std::ostream& streamOutputOperator(std::ostream& out) const override {
        out
                << *(this->nameAndVersion) << "\t"
                << Package::getName();

        return out;
    }

    bool lessThanOperator(const std::unique_ptr<Package>& anotherPackage) const override {
        return Package::getName() < anotherPackage->getName();
    }

//    bool lessThanOperator(const std::reference_wrapper<Package>& anotherPackage) override {
//        return Package::getName() < anotherPackage.get().getName();
//    }

private:
    std::unique_ptr<PackageNameAndVersion> nameAndVersion;

    uint_fast8_t getStartingPositionForPackageVersion() const;
};
