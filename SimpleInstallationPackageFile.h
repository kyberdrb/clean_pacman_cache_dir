//
// Created by laptop on 5/26/22.
//

#pragma once

#include "AbsolutePath.h"
#include "PackageName.h"
#include "PackageVersion.h"

class SimpleInstallationPackageFile {
public:
    explicit SimpleInstallationPackageFile(
            std::unique_ptr<AbsolutePath> absolutePath,
            std::string filename);

    const AbsolutePath& getAbsolutePath() const;
    const std::string& getFilename() const;

    friend std::ostream& operator<<(std::ostream& out, const SimpleInstallationPackageFile& packageFilename) {
        out << packageFilename.filename;
        out << "\t";
        out << *(packageFilename.absolutePath);
        return out;
    }

private:
    std::unique_ptr<AbsolutePath> absolutePath;
    std::string filename;
//    std::unique_ptr<Filename> filename;
};
