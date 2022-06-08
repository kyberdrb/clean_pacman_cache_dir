//
// Created by laptop on 6/1/22.
//

#pragma once

#include "PackageName.h"

class Package_refactored_ {
public:
    virtual void commonFunction() = 0;

    virtual ~Package_refactored_() = default;

    friend std::ostream& operator<<(std::ostream& out, const Package_refactored_& package) {
        return package.streamOutputOperator(out);
    }

protected:
    virtual std::ostream& streamOutputOperator(std::ostream& out) const = 0;

public:
    friend bool operator<(const std::unique_ptr<Package_refactored_>& package, const std::unique_ptr<Package_refactored_>& anotherPackage) {
        return package->lessThanOperator(anotherPackage);
    }

    // TODO maybe move 'std::unique_ptr<PackageName>' attribute here - to this abstract class, to centralize duplicate code?
    //  Then in constructors of derived classes use constructor delegation to initialize the instance of this abstract class
    virtual const PackageName& getName() const = 0;

protected:
    virtual bool lessThanOperator(const std::unique_ptr<Package_refactored_>& anotherPackage) const = 0;

public:
    friend bool operator<(const std::reference_wrapper<Package_refactored_>& package, const std::reference_wrapper<Package_refactored_>& anotherPackage) {
        return package.get().lessThanOperator(anotherPackage);
    }

protected:
    virtual bool lessThanOperator(const std::reference_wrapper<Package_refactored_>& anotherPackage) = 0;
};
