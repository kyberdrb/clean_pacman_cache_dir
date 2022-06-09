//
// Created by laptop on 6/9/22.
//

#pragma once

#include <cstdint>
#include <string>

class PackageNameAndVersion {
public:
    explicit PackageNameAndVersion(std::string packageNameAndVersionAsText);

    const std::string& string() const;

    std::string substr(uint_fast8_t fromPosition);

private:
    std::string nameAndVersion;
};
