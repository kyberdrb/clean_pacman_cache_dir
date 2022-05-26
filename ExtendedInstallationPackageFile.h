#pragma once

#include "PackageName.h"
#include "PackageVersion.h"

#include <memory>
#include <ostream>
#include <string>

class ExtendedInstallationPackageFile {
public:
    ExtendedInstallationPackageFile(std::string absolutePath, std::string filename, const PackageName& relatedPackageName, std::unique_ptr<PackageVersion> relatedPackageVersion);

    const std::string& getFilename() const;
    const std::string& getAbsolutePath() const;
    const PackageName& getRelatedPackageName() const;
    const PackageVersion& getRelatedPackageVersion() const;

    friend std::ostream& operator<<(std::ostream& out, const ExtendedInstallationPackageFile& packageFilename) {
        out << packageFilename.filename;
        out << "\t" << packageFilename.relatedPackageName;
        out << "\t" << *(packageFilename.packageVersionOfPackageFile);
        out << "\t" << packageFilename.absolutePath;
        return out;
    }

private:
    std::string absolutePath;
    std::string filename;
    const PackageName& relatedPackageName;
    std::unique_ptr<PackageVersion> packageVersionOfPackageFile;
};
