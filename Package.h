//
// Created by laptop on 6/1/22.
//

#pragma once

#include "PackageName.h"

class Package {
public:
    explicit Package(std::unique_ptr<PackageName> packageName);

    virtual ~Package() = default;

    const PackageName& getName() const;

    friend std::ostream& operator<<(std::ostream& out, const Package& package) {
        return package.streamOutputOperator(out);
    }

protected:
    virtual std::ostream& streamOutputOperator(std::ostream& out) const = 0;

public:
    friend bool operator<(const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) {
        return package->lessThanOperator(anotherPackage);
    }

protected:
    virtual bool lessThanOperator(const std::unique_ptr<Package>& anotherPackage) const = 0;

public:
    friend bool operator<(const std::reference_wrapper<Package>& package, const std::reference_wrapper<Package>& anotherPackage) {
        return package.get().lessThanOperator(anotherPackage);
    }

protected:
    virtual bool lessThanOperator(const std::reference_wrapper<Package>& anotherPackage) = 0;

protected:
    std::unique_ptr<PackageName> name;
};
