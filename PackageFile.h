#pragma once

#include "PackageName.h"
#include "PackageVersion.h"
#include "PackageWithInferredNameAndVersion.h"

#include <memory>
#include <ostream>
#include <string>

class PackageFile {
public:
    PackageFile(std::string absolutePath);

    PackageFile(std::string filename, std::string absolutePath, std::unique_ptr<PackageWithInferredNameAndVersion> packageWithInferredNameAndVersion);

    std::string getFilename() const;

    std::string getAbsolutePath() const;

    const PackageName& getRelatedPackageName() const;

    const PackageVersion& getRelatedPackageVersion() const;

    bool operator<(const PackageFile& packageFilename) const {
        return this->getFilename() < packageFilename.getFilename();
    }

    friend std::ostream& operator<<(std::ostream& os, const PackageFile& packageFilename) {
        // Print 'PackageFile' that had been constructed with the single argument constructor - for partially downloaded files
        if (packageFilename.filename.empty()) {
            os << packageFilename.absolutePath;
            return os;
        }

        os <<
            packageFilename.filename << "\t" <<
            packageFilename.relatedPackage->getName() << "\t" <<
            packageFilename.relatedPackage->getVersion() << "\t" <<
            packageFilename.absolutePath;
        return os;
    }

private:
    std::string filename;
    std::string absolutePath;
    std::unique_ptr<PackageWithInferredNameAndVersion> relatedPackage;
};
