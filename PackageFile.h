#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <vector>

class PackageFile {
public:
    PackageFile(std::string filename, std::string absolutePath);

    explicit PackageFile(std::string filename);

    PackageFile(std::string filename, std::string absolutePath, std::string relatedPackageName, std::string relatedPackageVersion);

    std::string getFilename() const;

    uint32_t isPartiallyMatchingInPrefix(const PackageFile& compoundPartialPackageFilenamePrefix) const;

    std::string getAbsolutePath() const;
    std::string getRelatedPackageName() const;
    std::string getRelatedPackageVersion() const;

    bool operator<(const PackageFile& packageFilename) const {
        return this->getFilename() < packageFilename.getFilename();
    }

    friend std::ostream& operator<<(std::ostream& os, const PackageFile& packageFilename) {
        os << packageFilename.filename << "\t" << packageFilename.extractedPackageNameFromFilename << "\t" << packageFilename.extractedPackageVersionFromFilename;
//        os << "\tisPackageSpecial: " << packageFilename.isPackageSpecial;
        return os;
    }

private:
    std::string filename;
    std::string absolutePath;
    std::string extractedPackageNameFromFilename;
    std::string extractedPackageVersionFromFilename;
//    bool isPackageSpecial;
    std::string relatedPackageVersion;
    std::string relatedPackageName;

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
