//
// Created by laptop on 4/28/22.
//

#include "PackageName.h"

PackageName::PackageName(std::string packageName) :
    name(std::move(packageName))
{}

bool PackageName::empty() const {
    return PackageName::name.empty();
}

const char& PackageName::at(size_t position) const {
    return PackageName::name.at(position);
}

size_t PackageName::size() const {
    return PackageName::name.size();
}

void PackageName::pop_back() {
    PackageName::name.pop_back();
}

const std::string& PackageName::string() const {
    return PackageName::name;
}
