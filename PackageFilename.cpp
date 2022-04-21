//
// Created by laptop on 4/21/22.
//

#include "PackageFilename.h"

PackageFilename::PackageFilename(std::string filename) :
        filename(filename)
{}

std::string PackageFilename::getFilename() const {
    return filename;
}

bool PackageFilename::isPartiallyMatchingInPrefix(const PackageFilename &compoundPartialPackageFilenamePrefix) const {
    int startingingPosition = 0;
    return this->filename.compare(startingingPosition, compoundPartialPackageFilenamePrefix.filename.size(), compoundPartialPackageFilenamePrefix.filename);
}
