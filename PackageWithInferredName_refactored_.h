//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package_refactored_.h"
#include "PackageVersion.h"

#include <iostream>

class PackageWithInferredName_refactored_ : public Package_refactored_ {
public:
    void commonFunction() override {
        std::cout << "Common function - from PackageWithInferredName__refactored__" << "\n";
    }

    void functionOnlyInPackageWithInferredName_refactored_() {
        std::cout << "Function only in PackageWithInferredName__refactored__" << "\n";
    }

    // For testing purposes
    PackageWithInferredName_refactored_() = default;

    explicit PackageWithInferredName_refactored_(std::unique_ptr<PackageName> inferredPackageName);

    const PackageName& getName() const override;

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    uint_fast8_t getStartingPositionForPackageVersion() const;

    std::unique_ptr<PackageVersion> extractPackageVersion();

//    friend bool operator<(const std::unique_ptr<PackageWithInferredName_refactored_>& package, const std::unique_ptr<PackageWithInferredName_refactored_>& anotherPackage) {
//        // Delegate comparison to overloaded 'operator<' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)' in 'PackageName' class
//        return package->name < anotherPackage->name;
//    }
//
//    friend bool operator<(
//            const std::reference_wrapper<PackageWithInferredName_refactored_>& package,
//            const std::reference_wrapper<PackageWithInferredName_refactored_>& anotherPackage)
//    {
//        return package.get().name < anotherPackage.get().name;
//    }

protected:
    std::ostream& streamOutputOperator(std::ostream& out) const override {
        out << *(this->name);

        if (locallyInstalledVersion.get() != nullptr) {
            out << *(this->locallyInstalledVersion);
        }

        return out;
    }

    bool lessThanOperator(const std::unique_ptr<Package_refactored_>& anotherPackage) const override {
//        return *(const_cast <const PackageName*> (this->name.get() ) ) < anotherPackage->getName();
        return *(this->name) < anotherPackage->getName();
    }

    bool lessThanOperator(const std::reference_wrapper<Package_refactored_>& anotherPackage) override {
        return *(this->name) < anotherPackage.get().getName();
    }

private:
    std::unique_ptr<PackageName> nameAndVersion;
    std::unique_ptr<PackageName> name;
    std::unique_ptr<PackageVersion> locallyInstalledVersion;
};
