//
// Created by laptop on 6/1/22.
//

#pragma once

#include "PackageName.h"

class Package_refactored_ {
public:
//    explicit Package_refactored_(std::unique_ptr<PackageName> packageName);

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

    virtual const PackageName& getName() const = 0;
    // TODO maybe move 'std::unique_ptr<PackageName>' attribute here - to this abstract class, to centralize duplicate code?
    //  Then in constructors of derived classes use constructor delegation to initialize the instance of this abstract class
//    const PackageName& Package_refactored_::getName() const;

protected:
    virtual bool lessThanOperator(const std::unique_ptr<Package_refactored_>& anotherPackage) const = 0;

public:
    friend bool operator<(const std::reference_wrapper<Package_refactored_>& package, const std::reference_wrapper<Package_refactored_>& anotherPackage) {
        return package.get().lessThanOperator(anotherPackage);
    }

protected:
    virtual bool lessThanOperator(const std::reference_wrapper<Package_refactored_>& anotherPackage) = 0;

//private:
//    std::unique_ptr<PackageName> name;
};
