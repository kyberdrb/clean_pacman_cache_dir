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

    friend bool operator!=(const PackageVersion& packageVersion, const PackageVersion& anotherPackageVersion) {
        return packageVersion.getVersion() != anotherPackageVersion.getVersion();
    }

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

// Fails at linking with error message:
//   "multiple definition of `operator!=(PackageVersion const&, PackageVersion const&)'; CMakeFiles/clean_pacman_cache_dir.dir/main.cpp.o:/home/laptop/git/kyberdrb/clean_pacman_cache_dir/PackageVersion.h:43: first defined here"
//  prefixing function header with 'inline' keyword solves the issue, because multiple inline definition are allowed as long as they have the same function body
//bool operator!=(const PackageVersion& packageVersion, const PackageVersion& anotherPackageVersion) {
//    return packageVersion.getVersion() != anotherPackageVersion.getVersion();
//}

//inline bool operator!=(const PackageVersion& packageVersion, const PackageVersion& anotherPackageVersion) {
//    return packageVersion.getVersion() != anotherPackageVersion.getVersion();
//}
