#pragma once

#include "AbsolutePath.h"
#include "PackageName.h"
#include "PackageVersion.h"

class ExtendedInstallationPackageFile {
public:
    ExtendedInstallationPackageFile(
            std::unique_ptr<AbsolutePath> absolutePath,
            std::string filename,
            const PackageName& relatedPackageName,
            std::unique_ptr<PackageVersion> relatedPackageVersion);

    const std::string& getFilename() const;
    const AbsolutePath& getAbsolutePath() const;
    const PackageName& getRelatedPackageName() const;
    const PackageVersion& getRelatedPackageVersion() const;

    friend std::ostream& operator<<(std::ostream& out, const ExtendedInstallationPackageFile& packageFilename) {
        out << packageFilename.filename;
        out << "\t" << packageFilename.relatedPackageName;
        out << "\t" << *(packageFilename.relatedPackageVersion);
        out << "\t" << *(packageFilename.absolutePath);
        return out;
    }

private:
    std::unique_ptr<AbsolutePath> absolutePath;
    std::string filename;
//    std::unique_ptr<Filename> filename;
    const PackageName& relatedPackageName;
    std::unique_ptr<PackageVersion> relatedPackageVersion;
};
