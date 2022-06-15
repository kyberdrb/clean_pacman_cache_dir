//
// Created by laptop on 6/10/22.
//

#include "Package.h"

Package::Package(std::unique_ptr<PackageName> packageName) :
    name(std::move(packageName))
{}

const PackageName& Package::getName() const {
    return *(this->name);
}
