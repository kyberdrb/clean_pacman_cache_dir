#pragma once

#include <memory>
#include <ostream>
#include <string>

class PackageFile {
public:
    PackageFile(std::string absolutePath);

    PackageFile(std::string filename, std::string absolutePath, std::string relatedPackageName, std::string relatedPackageVersion);

    std::string getFilename() const;

    std::string getAbsolutePath() const;
    std::string getRelatedPackageName() const;
    std::string getRelatedPackageVersion() const;

    bool operator<(const PackageFile& packageFilename) const {
        return this->getFilename() < packageFilename.getFilename();
    }

    friend std::ostream& operator<<(std::ostream& os, const PackageFile& packageFilename) {
        if (packageFilename.filename.empty()) {
            os << packageFilename.absolutePath;
            return os;
        }

        os << packageFilename.filename << "\t" << packageFilename.relatedPackageName << "\t" << packageFilename.relatedPackageVersion << "\t" << packageFilename.absolutePath;
        return os;
    }

private:
    std::string filename;
    std::string absolutePath;
    std::string relatedPackageName;
    std::string relatedPackageVersion;
};
