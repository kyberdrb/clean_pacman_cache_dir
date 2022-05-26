//
// Created by laptop on 5/26/22.
//

#include "SimpleInstallationPackageFile.h"

// TODO pass filename by value from 'main' or deduce the filename by reversing the 'absolutePath'
SimpleInstallationPackageFile::SimpleInstallationPackageFile(std::string absolutePath, std::string filename) :
        absolutePath(std::move(absolutePath)),
        filename(std::move(filename))
{}

std::string SimpleInstallationPackageFile::getAbsolutePath() const {
    return this->absolutePath;
}

std::string SimpleInstallationPackageFile::getFilename() const {
    return this->filename;
}
