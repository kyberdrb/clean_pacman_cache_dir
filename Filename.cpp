//
// Created by laptop on 5/30/22.
//

#include "Filename.h"

#include <algorithm>
#include <sstream>

Filename::Filename(std::string filename) :
    nameOfFile(std::move(filename))
{}

const std::string& Filename::getFilename() const {
    return this->nameOfFile;
}

std::string Filename::extractPackageNameAndVersion() {
    std::reverse(this->nameOfFile.begin(), this->nameOfFile.end());

    std::stringstream packageNameAndVersionReversed{};
    char delimiter = '-';
    bool stillSearchingForFirstDelimiterOccurence = true;

    //find the first dash '-' in the reversed filename and append everything after
    for (auto character : this->nameOfFile) {
        if (character != delimiter && stillSearchingForFirstDelimiterOccurence) {
            continue;
        }

        stillSearchingForFirstDelimiterOccurence = false;
        packageNameAndVersionReversed << character;
    }

    // reverse the package filename back - by getting the filename as getAbsolutePath again?
    std::reverse( this->nameOfFile.begin(), this->nameOfFile.end() );

    auto packageNameAndVersion = packageNameAndVersionReversed.str();

    std::reverse(packageNameAndVersion.begin(), packageNameAndVersion.end());
    packageNameAndVersion.pop_back();

    return packageNameAndVersion;
}