//
// Created by laptop on 6/17/22.
//

#include "IgnoredPackageNames.h"
#include "IgnoredPackageNameComparatorPredicate.h"

#include <algorithm>
#include <fstream>
#include <regex>
#include <sstream>

IgnoredPackageNames::IgnoredPackageNames() {
    // TODO encapsulate pacman's config file parsing to a separate class e.g. 'PacmanConfigParser' to function e.g. 'getIgnoredPackageNames'

    // 'alpm_option_get_ignorepkgs' to retrieve the list of ignored packages from pacman's config doesn't work. Parsing '/etc/pacman.conf' manually
    //alpm_list_t* listOfIgnoredPackages = alpm_option_get_ignorepkgs(handle);

    std::ifstream pacmanConfigFile;

    // TODO parametrize with argument (maybe use getopt?) - if parameter empty, then use default one + check whether the pacman configuration file in the default path actually exists; otherwise exit?/ask user whether to terminate or continue, because the configuration file is used to determine ignored packages in order to exclude them from deletion
    pacmanConfigFile.open("/etc/pacman.conf");

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

    auto lineWithIgnoredPackagesWithoutOptionPrefix = regex_replace(lineWithIgnoredPackages, regexForIgnoredPackagesInPacmanConfigFile, "");
    std::stringstream ignoredPackagesAsStream{lineWithIgnoredPackagesWithoutOptionPrefix};
    std::string ignoredPackageNameAsText{};
    char delimiterForIgnoredPackages = ' ';

    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPackages)) {
        auto ignoredPackageName = std::make_unique<IgnoredPackageName>(std::move(ignoredPackageNameAsText));
        this->ignoredPackageNames.emplace_back(std::move(ignoredPackageName));
    }
}

void IgnoredPackageNames::emplace_back(std::unique_ptr<IgnoredPackageName> ignoredPackageName) {
    this->ignoredPackageNames.emplace_back(std::move(ignoredPackageName));
}

std::string IgnoredPackageNames::generateReport() const {
    std::stringstream stringstream{};
    
    stringstream << "\n";
    stringstream << "===============================================\n\n";
    stringstream << "LIST OF IGNORED PACKAGES\n\n";

    stringstream << "Found " << this->ignoredPackageNames.size() << " ignored packages\n\n";

    for (const auto& ignoredPackageName : this->ignoredPackageNames) {
        stringstream << *ignoredPackageName  << "\n";
    }

    return stringstream.str();
}

bool IgnoredPackageNames::isPackageWithGivenNameIgnored(std::unique_ptr<IgnoredPackageName>& ignoredPackageNameCandidate) const {
    return std::any_of(
            this->ignoredPackageNames.begin(),
            this->ignoredPackageNames.end(),
            IgnoredPackageNameComparatorPredicate(ignoredPackageNameCandidate));
}
