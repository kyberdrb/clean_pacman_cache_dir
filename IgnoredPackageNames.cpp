//
// Created by laptop on 5/1/22.
//

#include "IgnoredPackageNames.h"

#include <fstream>
#include <regex>

// Populate ignoredPackageNames collection immediately in constructor - constructor parameter for PacmanConfigurationFilePath will be optional; when ommitted, the default path will be "/etc/pacman.conf"
IgnoredPackageNames::IgnoredPackageNames(std::string pacmanConfigurationFilePath) :
        pacmanConfigurationFilePath(std::move(pacmanConfigurationFilePath))
{
    this->setDefaultPacmanConfigPathIfEmpty();
    this->findIgnoredPackageNames();
}

IgnoredPackageNames::IgnoredPackageNames() :
        IgnoredPackageNames(std::string{})
{}

void IgnoredPackageNames::findIgnoredPackageNames() {
    std::ifstream pacmanConfigFile;

    //  'alpm_option_get_ignorepkgs' to retrieve the list of ignored packages from pacman's config doesn't work. Parsing '/etc/pacman.conf' manually
    //    alpm_list_t* listOfIgnoredPackages = alpm_option_get_ignorepkgs(handle);

    // TODO parametrize with argument (maybe use getopt?) - if parameter empty, then use default one + check whether the pacman configuration file in the default path actually exists; otherwise exit?/ask user whether to terminate or continue, because the configuration file is used to determine ignored packages in order to exclude them from deletion
    pacmanConfigFile.open(this->pacmanConfigurationFilePath);

    std::string lineWithIgnoredPackages;
    std::smatch match;
    std::regex regexForIgnoredPackagesInPacmanConfigFile("^IgnorePkg = ");

    while (std::getline(pacmanConfigFile, lineWithIgnoredPackages)) {
        bool doesTheLineContainIgnoredPackages = std::regex_search(lineWithIgnoredPackages, match, regexForIgnoredPackagesInPacmanConfigFile);
        if (doesTheLineContainIgnoredPackages) {
            break;
        }
    }

    // TODO OPTIONAL (assuming no leading spaces/tabs) remove leading and ending blank characters
    // TODO OPTIONAL (assuming no ending spaces/tabs; only one space delimiting [separating] each package filename) replace multiple spaces or tabs with one space
    // build a list of ignored packages

    auto lineWithIgnoredPackagesWithoutOptionPrefix = regex_replace(lineWithIgnoredPackages, regexForIgnoredPackagesInPacmanConfigFile, "");
    std::stringstream ignoredPackagesAsStream{lineWithIgnoredPackagesWithoutOptionPrefix};
    std::string ignoredPackageNameAsText{};
    char delimiterForIgnoredPakcages = ' ';

    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPakcages)) {
        this->ignoredPackageNames.push_back(std::make_unique<PackageName>(ignoredPackageNameAsText) );
    }
}

std::string IgnoredPackageNames::generateReport() const {
    std::stringstream reportStream{};
    reportStream << *this;
    return reportStream.str();
}

void IgnoredPackageNames::setDefaultPacmanConfigPathIfEmpty() {
    if (this->pacmanConfigurationFilePath.empty()) {
        const std::string DEFAULT_PACMAN_CONFIGURATION_FILE_PATH = "/etc/pacman.conf";
        this->pacmanConfigurationFilePath = DEFAULT_PACMAN_CONFIGURATION_FILE_PATH;
    }
}
