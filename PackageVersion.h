//
// Created by laptop on 4/28/22.
//

#pragma once

#include <memory>
#include <ostream>

class PackageVersion {
public:
    explicit PackageVersion(std::string packageVersionAsText);

    bool empty() const;
    const char& at(size_t position);

    const std::string& getVersion() const;

    friend std::ostream& operator<<(std::ostream& out, const PackageVersion& packageVersion) {
        out << packageVersion.version;
        return out;
    }

//    friend bool operator!=(const std::unique_ptr<PackageVersion>& packageVersion, const std::string& anotherPakcageVersion) {
//        return packageVersion->getVersion() != anotherPakcageVersion;
//    }

//    bool operator!=(const std::unique_ptr<PackageVersion>& packageVersion, const std::string& anotherPakcageVersion);

//    bool operator!=(const std::unique_ptr<PackageVersion>& packageVersion) const {
//        return this->version != packageVersion->getVersion();
//    }

//    bool operator!=(const std::string& packageVersionAsText) const {
//        return this->version != packageVersionAsText;
//    }

private:
    std::string version;
};

//bool operator!=(const std::unique_ptr<PackageVersion>& packageVersion, const std::string& anotherPakcageVersion) {
//    return packageVersion->getVersion() != anotherPakcageVersion;
//}

inline bool operator!=(const PackageVersion& packageVersion, const PackageVersion& anotherPackageVersion) {
    return packageVersion.getVersion() != anotherPackageVersion.getVersion();
}
