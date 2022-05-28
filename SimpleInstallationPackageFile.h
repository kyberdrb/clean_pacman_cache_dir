//
// Created by laptop on 5/26/22.
//

#pragma once

#include "PackageName.h"
#include "PackageVersion.h"

class SimpleInstallationPackageFile {
public:
    explicit SimpleInstallationPackageFile(std::string absolutePath, std::string filename);

    const std::string& getAbsolutePath() const;
    const std::string& getFilename() const;

    friend std::ostream& operator<<(std::ostream& out, const SimpleInstallationPackageFile& packageFilename) {
        out << packageFilename.filename;
        out << "\t";
        out << packageFilename.absolutePath;
        return out;
    }

private:
    std::string absolutePath;
//    std::unique_ptr<AbsolutePath> absolutePath;
    std::string filename;
//    std::unique_ptr<Filename> filename;
};
