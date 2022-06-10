//
// Created by laptop on 6/10/22.
//

#include "Package_refactored_.h"

Package_refactored_::Package_refactored_(std::unique_ptr<PackageName> packageName) :
    name(std::move(packageName))
{}

const PackageName& Package_refactored_::getName() const {
    return *(this->name);
}
