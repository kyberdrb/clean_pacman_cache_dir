#include "MoverOfInstallationPackageFiles.h"
#include "TerminalSingleton.h"

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
    // Rely on the implicit move semantics - don't move the function's return value explicitly to allow compiler to do Move Elision
    std::stringstream message;
    message << ignoredPackageNames->generateReport()
            << locallyInstalledPackages->generateReport()
            << matchFinderWithPackageFilesRelatedToPackages->generateReport();
    TerminalSingleton::get().printText(message);

    // MOVE PACKAGE FILES TO SEPARATE DIRECTORY
    auto installationPackageFilesMover = std::make_unique<MoverOfInstallationPackageFiles>(
            *matchFinderWithPackageFilesRelatedToPackages,
            *locallyInstalledPackages);
//    auto installationPackageFilesMover = std::make_unique<MoverOfInstallationPackageFiles>(
//            *locallyInstalledPackages,
//            *matchFinderWithPackageFilesRelatedToPackages,
//            directoryForMovedInstallationPackageFiles);

    installationPackageFilesMover->moveChosenInstallationPackageFilesToSeparateDir();

    return 0;
}
