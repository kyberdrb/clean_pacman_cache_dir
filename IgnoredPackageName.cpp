//
// Created by laptop on 5/7/22.
//

#include "IgnoredPackageName.h"

IgnoredPackageName::IgnoredPackageName(std::string ignoredPackageName) :
        name(std::move(ignoredPackageName))
{}

std::string&& IgnoredPackageName::moveNameFromThisInstance() {
    return std::move(this->name);
}
