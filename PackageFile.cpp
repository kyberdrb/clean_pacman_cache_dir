//
// Created by laptop on 4/21/22.
//

#include "PackageFile.h"

#include <regex>

PackageFile::PackageFile(std::string filename, std::string absolutePath) :
        filename(std::move(filename)),
        absolutePath(absolutePath)
{
    setPackageNameAndVersionFromFilename();
}

// for creating PackageFiles with partial filename prefix to be used with 'lower_bound' function in map
//  where it doesn't make sense to set package name and version for instances of this class
//  by delegating to another constructor,
//  because the partial package filname prefix already comes from the package list
PackageFile::PackageFile(std::string filename) :
        filename(std::move(filename)),
        absolutePath(" ")
{}

void PackageFile::setPackageNameAndVersionFromFilename() {
    // REGEX REPLACE TO REPLACING THE TRAILING PART OF PACKAGE FILENAMES
    //  leaving only package filename and package version

    std::string pkgFilename = this->filename;
    //std::cout << "Package filename:\n";
    //std::cout << pkgFilename << "\n";

    // strip the extensions and architecture from package filename and leave only package version and name
    //  I couldn't find out how to do first-match or non-greedy replace with regex_replace,
    //  so I'm writing my own algorithm
    std::reverse(pkgFilename.begin(), pkgFilename.end());
    std::stringstream pkgNameAndVersionReversed{};
    char delimiter = '-';
    bool stillSearchingForFirstDelimiterOccurence = true;

    //find the first dash '-' in the reversed filename and append everything after
    for (auto character : pkgFilename) {
        if (character != delimiter && stillSearchingForFirstDelimiterOccurence) {
            continue;
        }

        stillSearchingForFirstDelimiterOccurence = false;
        pkgNameAndVersionReversed << character;
    }

    auto pkgNameAndVersion = pkgNameAndVersionReversed.str();


    std::reverse(pkgNameAndVersion.begin(), pkgNameAndVersion.end());

    // TODO breaks when creating an instance with partial package filename to be used with lower_bound
    pkgNameAndVersion.pop_back();

    //std::cout << pkgNameAndVersion << "\n";

    std::stringstream pkgFilenameAsStream{pkgNameAndVersion};
    std::string token{};
    std::vector<std::string> tokens{};

    while(getline(pkgFilenameAsStream, token, delimiter)) {
        tokens.push_back(token);
    }

    // for debugging purposes - pakcage with longer version
//            if (pkgNameAndVersion == "adobe-source-code-pro-fonts-2.038ro+1.058it+1.018var-1") {
//                auto p = static_cast<std::string*>(&pkgNameAndVersion);
//            }

    std::string packageName{};
    std::string packageVersion{};

    bool hasPackageNameMoreTokens = true;

    // assuming package filename format
    //  (\w.*-?)*(\d.*-?)*
    //  ˇˇˇˇˇˇˇˇˇ.........
    //  package  package
    //  filename     version

    for (const auto& token : tokens) {
        bool isFirstCharacterOfTokenLetter = !isdigit(token.at(0));

        if (isFirstCharacterOfTokenLetter && hasPackageNameMoreTokens) {
            packageName += token + delimiter;
            continue;
        }

        hasPackageNameMoreTokens = false;

        packageVersion += token + delimiter;
    }

    // TODO handle special package names:
    //   if the first character packageName is a number
    //     create a <PackageName, Package> pair and add it to the specialPackages map [packageName will be empty - all will be saved in the packageVersion]
    //  For multi-word package names:
    //   tokenize the packageName by dashes
    //   for each token in tokens
    //     when the first character of the token is a number
    //       create a <PackageName, Package> pair and add it to the specialPackages map [packageName will be incomplete - the rest of the package filename will be save in the packageVersion]

    // TODO handle special package versions:
    //   if the first character packageVersion is a letter
    //     create a <PackageName, Package> pair and add it to the specialPackages map [packageVersion may be empty or incomplete - the rest will be saved in the packageName]
    //  For multi-word package versions (e.g. with included release version):
    //   tokenize the packageVersion by dashes
    //   for each token in tokens
    //     when the first character of the token is a letter
    //       create a <PackageName, Package> pair and add it to the specialPackages map [packageName may be empty or incomplete - the rest of the package version will be save in the packageName]
    //  Note for handling special packages: in (rare) cases there occure package names with numbers as the first character e.g. '0ad' and  after the dash e.g. '-1.16'
    //   and package version that begin with a letter e.g. '	a25.b-3'
    //    - adjust 'operator<'?
    //    - write a custom comparator that will check character by character for prefix similarity?
    //    - add them to ignoredPackages?
    //    - add them to unhandled/other/special packages and then handle manually?/automatically? << MAYBE THIS WILL BE THE OPTION I'll try out
    //    - assemble a package filename by trial-and-error from available extensions and check whether the file under assembles filename exists?
    //  Problematic package filenames use-cases:
    //   libyuv-r2212+dfaf7534-2-x86_64.pkg.tar.zst
    //   libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst
    //   libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst.sig
    //  the parsing algorithm breaks when the first character of any of the tokens belonging to the package filename is a number
    //   - then packageName stays empty or incomplete, the packageVersion has the rest of the package filename together with the version
    //   or when the package version has as the first token a letter

    packageName.pop_back();
    packageVersion.pop_back();
    bool isPackageSpecial = false;

    if ( ( std::isdigit(packageName.at(0) ) ) || ( ! std::isdigit(packageVersion.at(0) ) ) ) {
        isPackageSpecial = true;
    }

    this->extractedPackageNameFromFilename = packageName;

    this->extractedPackageVersionFromFilename = packageVersion;
}

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
