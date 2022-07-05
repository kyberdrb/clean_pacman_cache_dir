//
// Created by laptop on 4/28/22.
//

//
// Created by laptop on 5/26/22.
//

#pragma once

#include <memory>
#include <ostream>

class PackageVersion {
public:
    explicit PackageVersion(std::string packageVersionAsText);

    const std::string& getVersion() const;

    friend std::ostream& operator<<(std::ostream& out, const PackageVersion& packageVersion) {
        out << packageVersion.version;
        return out;
    }

    bool operator!=(const PackageVersion& otherPackageVersion) const {
        return this->version != otherPackageVersion.getVersion();
    }

private:
    std::string version;
};
