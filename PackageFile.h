#pragma once

#include "PackageName.h"

#include <memory>
#include <ostream>
#include <string>

class PackageFile {
public:
    explicit PackageFile(std::string absolutePath);

    PackageFile(std::string filename, std::string absolutePath, const PackageName& relatedPackageName, std::string relatedPackageVersion);

    std::string getFilename() const;

    std::string getAbsolutePath() const;
    const PackageName& getRelatedPackageName() const;
    std::string getRelatedPackageVersion() const;

    friend std::ostream& operator<<(std::ostream& os, const PackageFile& packageFilename) {
        if (packageFilename.filename.empty()) {
            os << packageFilename.absolutePath;
            return os;
        }

        os << packageFilename.filename;

        // TODO 'relatedPackageName' refers to a temporary when PackageFile is constructed with the single argument 'absolutePath'
        //  constructor, which may lead to undefined behavior when dereferenced here.
        //  For now it's sufficient to check whether the 'filename' is empty
        os << "\t" << packageFilename.relatedPackageName;

        os << "\t" << packageFilename.relatedPackageVersion;
        os << "\t" << packageFilename.absolutePath;

        return os;
    }

private:
    std::string filename;
    std::string absolutePath;
    const PackageName& relatedPackageName;
    std::string relatedPackageVersion;
};
