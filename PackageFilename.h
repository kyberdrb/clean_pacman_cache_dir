#pragma once

#include <memory>
#include <string>
#include <ostream>

class PackageFilename {
public:
    explicit PackageFilename(std::string filename);

    std::string getFilename() const;

    bool isPartiallyMatchingInPrefix(const PackageFilename& compoundPartialPackageFilenamePrefix) const;

    bool operator<(const PackageFilename& packageFilename) const {
        return this->getFilename() < packageFilename.getFilename();
    }

    friend std::ostream &operator<<(std::ostream &os, const PackageFilename &packageFilename) {
        os << packageFilename.filename;
        return os;
    }

private:
    std::string filename;
};

namespace std {
    template<>
    struct less<unique_ptr<PackageFilename>> {
        bool operator() (const unique_ptr<PackageFilename>& lhs, const unique_ptr<PackageFilename>& rhs) const {
            return *lhs < *rhs;
        }
    };
}
