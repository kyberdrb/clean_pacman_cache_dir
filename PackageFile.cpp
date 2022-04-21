//
// Created by laptop on 4/21/22.
//

//#include <regex>
#include "PackageFile.h"

PackageFile::PackageFile(std::string filename, std::string absolutePath) :
        filename(filename),
        absolutePath(absolutePath)
{}

PackageFile::PackageFile(std::string filename) :
    PackageFile(std::move(filename), " ")
{}

std::string PackageFile::getFilename() const {
    return filename;
}

uint32_t PackageFile::isPartiallyMatchingInPrefix(const PackageFile& compoundPartialPackageFilenamePrefix) const {
    int startingingPosition = 0;
    return this->filename.compare(startingingPosition, compoundPartialPackageFilenamePrefix.filename.size(), compoundPartialPackageFilenamePrefix.filename);

    //auto compoundPartialPackageFilenamePrefixEscaped = std::regex_replace();    // Replace all: .:

//    std::regex regexForIgnoredPackagesInPacmanConfigFile("^" + compoundPartialPackageFilenamePrefix.filename + ".*");
//    std::smatch match{};
//    return std::regex_search(this->filename, match, regexForIgnoredPackagesInPacmanConfigFile);
}

std::string PackageFile::getAbsolutePath() const {
    return absolutePath;
}
