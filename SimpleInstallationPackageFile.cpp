//
// Created by laptop on 5/26/22.
//

#include "SimpleInstallationPackageFile.h"

SimpleInstallationPackageFile::SimpleInstallationPackageFile(std::string absolutePath, std::string filename) :
        absolutePath(std::move(absolutePath)),
        filename(std::move(filename))
{}

const std::string& SimpleInstallationPackageFile::getAbsolutePath() const {
    return this->absolutePath;
}

const std::string& SimpleInstallationPackageFile::getFilename() const {
    return this->filename;
}
