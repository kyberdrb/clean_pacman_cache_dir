//
// Created by laptop on 6/9/22.
//

#pragma once

#include <cstdint>
#include <string>
#include <ostream>

class PackageNameAndVersion {
public:
    explicit PackageNameAndVersion(std::string packageNameAndVersionAsText);

    const std::string& string() const;

    std::string substr(uint_fast8_t fromPosition);

    friend std::ostream& operator<<(std::ostream& out, const PackageNameAndVersion& version);

private:
    std::string nameAndVersion;
};
