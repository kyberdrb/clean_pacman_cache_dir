#pragma once

#include "PackageName.h"
#include "PackageVersion.h"

#include <memory>
#include <ostream>
#include <string>

class ExtendedInstallationPackageFile {
public:
    explicit ExtendedInstallationPackageFile(std::string absolutePath);

    ExtendedInstallationPackageFile(std::string filename, std::string absolutePath, const PackageName& relatedPackageName, std::unique_ptr<PackageVersion> relatedPackageVersion);

    std::string getFilename() const;
    std::string getAbsolutePath() const;
    const PackageName& getRelatedPackageName() const;
    const PackageVersion& getRelatedPackageVersion() const;

    friend std::ostream& operator<<(std::ostream& out, const ExtendedInstallationPackageFile& packageFilename) {
        if (packageFilename.filename.empty()) {
            out << packageFilename.absolutePath;
            return out;
        }

        out << packageFilename.filename;

        // TODO 'relatedPackageName' refers to a temporary when ExtendedInstallationPackageFile is constructed with the single argument 'absolutePath'
        //  constructor, which may lead to undefined behavior when dereferenced here.
        //  For now it's sufficient to check whether the 'filename' is empty
        out << "\t" << packageFilename.relatedPackageName;

        out << "\t" << *(packageFilename.packageVersionOfPackageFile);
        out << "\t" << packageFilename.absolutePath;

        return out;
    }

private:
    std::string filename;
    std::string absolutePath;
    const PackageName& relatedPackageName;
    std::unique_ptr<PackageVersion> packageVersionOfPackageFile;
};
