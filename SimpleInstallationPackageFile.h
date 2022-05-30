//
// Created by laptop on 5/26/22.
//

#pragma once

#include "AbsolutePath.h"
#include "PackageName.h"
#include "PackageVersion.h"
#include "SimpleInstallationPackageFileType.h"

class SimpleInstallationPackageFile {
public:
    explicit SimpleInstallationPackageFile(
            std::unique_ptr<AbsolutePath> absolutePath,
            std::unique_ptr<Filename> filename,
            SimpleInstallationPackageFileType reasonForDeletion);

    const AbsolutePath& getAbsolutePath() const;
    const Filename& getFilename() const;

    void moveToSeparateDirectoryForDeletion(const AbsolutePath& directoryForInstallationPackageFilesForDeletion);

    friend std::ostream& operator<<(std::ostream& out, const SimpleInstallationPackageFile& simpleInstallationPackageFile) {
        out << *(simpleInstallationPackageFile.filename);
        out << "\t";
        out << *(simpleInstallationPackageFile.absolutePath);
        return out;
    }

private:
    std::unique_ptr<AbsolutePath> absolutePath;
    std::unique_ptr<Filename> filename;
    SimpleInstallationPackageFileType reasonForDeletion;
};
