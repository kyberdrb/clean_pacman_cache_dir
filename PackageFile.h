#pragma once

#include "PackageName.h"
#include "PackageVersion.h"

#include <memory>
#include <ostream>
#include <string>

class PackageFile {
public:
    PackageFile(std::string absolutePath);

    PackageFile(std::string filename, std::string absolutePath, std::string relatedPackageName, std::string relatedPackageVersion);
//    PackageFile(std::string filename, std::string absolutePath, std::unique_ptr<Package> packageWithInferredNameAndVersion);

    std::string getFilename() const;

    std::string getAbsolutePath() const;

    const PackageName& getRelatedPackageName() const;

    const PackageVersion& getRelatedPackageVersion() const;
//    const std::string& getRelatedPackageVersion() const;

    bool operator<(const PackageFile& packageFilename) const {
        return this->getFilename() < packageFilename.getFilename();
    }

    friend std::ostream& operator<<(std::ostream& os, const PackageFile& packageFilename) {
        os << packageFilename.filename << "\t" << *(packageFilename.relatedPackageName) << "\t" << *(packageFilename.relatedPackageVersion) << "\t" << packageFilename.absolutePath;
        return os;
    }

private:
    std::string filename;
    std::string absolutePath;

//    std::string relatedPackageName;
    std::unique_ptr<PackageName> relatedPackageName;

//    std::string relatedPackageVersion;
    std::unique_ptr<PackageVersion> relatedPackageVersion;
};
