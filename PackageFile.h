#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

class PackageFile {
public:
    explicit PackageFile(std::string filename, std::string absolutePath);

    PackageFile(std::string filename);

    std::string getFilename() const;

    uint32_t isPartiallyMatchingInPrefix(const PackageFile& compoundPartialPackageFilenamePrefix) const;

    std::string getAbsolutePath() const;

    bool operator<(const PackageFile& packageFilename) const {
        return this->getFilename() < packageFilename.getFilename();
    }

    friend std::ostream &operator<<(std::ostream &os, const PackageFile &packageFilename) {
        os << packageFilename.filename;
        return os;
    }

private:
    std::string filename;
    std::string absolutePath;
    std::string extractedPackageNameFromFilename;
    std::string extractedPackageVersionFromFilename;

    void setPackageNameAndVersionFromFilename();
};

// overload the 'less' functor in order to enable lookup ('find') in map with instances of this class as a key
namespace std {
    template<>
    struct less<unique_ptr<PackageFile>> {
        bool operator() (const unique_ptr<PackageFile>& lhs, const unique_ptr<PackageFile>& rhs) const {
            return *lhs < *rhs;
        }
    };
}
