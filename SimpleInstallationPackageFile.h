//
// Created by laptop on 5/26/22.
//

#pragma once

#include "PackageName.h"
#include "PackageVersion.h"

#include <memory>
#include <ostream>
#include <string>

class SimpleInstallationPackageFile {
public:
    explicit SimpleInstallationPackageFile(std::string absolutePath, std::string filename);

    std::string getAbsolutePath() const;
    std::string getFilename() const;

    friend std::ostream& operator<<(std::ostream& out, const SimpleInstallationPackageFile& packageFilename) {
        out << packageFilename.filename;
        out << "\t";
        out << packageFilename.absolutePath;
        return out;
    }

private:
    std::string absolutePath;
    std::string filename;
};
