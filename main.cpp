#include "MoverOfInstallationPackageFile.h"

#include <iostream>

//#include <cassert>

int main() {
    // FIND IGNORED PACKAGES - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES
    auto ignoredPackageNames = std::make_unique<IgnoredPackageNames>();

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES
    auto locallyInstalledPackages = std::make_unique<LocallyInstalledPackages>(*ignoredPackageNames);

    // RELATE PACKAGE FILES TO THEIR RESPECTIVE PACKAGES
    auto matchFinderWithPackageFilesRelatedToPackages = std::make_unique<MatchFinderForPackageFilesToLocallyInstalledPackages>(*locallyInstalledPackages);
    // TODO relate installation package files from multiple cache directories, e. g. pacman's and pikaur's
//    auto matchFinderWithPackageFilesRelatedToPackages = std::make_unique<MatchFinderForPackageFilesToLocallyInstalledPackages>(*locallyInstalledPackages, installationPackageFilesCacheDirs);

    // SHOW REPORT
    // TODO replace with:
    //     TerminalPrinter::print(ignoredPackageNames->generateReport());
    std::cout << ignoredPackageNames->generateReport();
    std::cout << locallyInstalledPackages->generateReport();
    std::cout << matchFinderWithPackageFilesRelatedToPackages->generateReport();

    // MOVE PACKAGE FILES TO SEPARATE DIRECTORY
    auto installationPackageFileMover = std::make_unique<MoverOfInstallationPackageFile>(
            *matchFinderWithPackageFilesRelatedToPackages,
            *locallyInstalledPackages);
    //auto installationPackageFileMover = std::make_unique<MoverOfInstallationPackageFile>(*locallyInstalledPackages, *matchFinderWithPackageFilesRelatedToPackages, directoryForMovedInstallationPackageFiles);

    installationPackageFileMover->moveChosenInstallationPackageFilesToSeparateDir();

    return 0;
}
