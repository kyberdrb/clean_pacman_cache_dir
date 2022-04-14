//
// Created by laptop on 4/13/22.
//

#include "PackageName.h"

PackageName::PackageName(std::string name) :
    name(name)
{}

std::string PackageName::getName() const {
    return name;
}
