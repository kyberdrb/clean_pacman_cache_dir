// CPMCD - Clean Package Manager's Cache Directories

#include "MoverOfInstallationPackageFiles.h"
#include "TerminalAndLoggerSingleton.h"

int main() {
    // FIND IGNORED PACKAGES - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES
    auto ignoredPackageNames = std::make_unique<IgnoredPackageNames>();

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES
    auto locallyInstalledPackages = std::make_unique<LocallyInstalledPackages>(*ignoredPackageNames);

    // RELATE PACKAGE FILES TO THEIR RESPECTIVE PACKAGES
    auto matchFinderWithPackageFilesRelatedToPackages = std::make_unique<MatchFinderForPackageFilesToLocallyInstalledPackages>(*locallyInstalledPackages);
//    auto matchFinderWithPackageFilesRelatedToPackages = std::make_unique<MatchFinderForPackageFilesToLocallyInstalledPackages>(*locallyInstalledPackages, installationPackageFilesCacheDirs);

    // SHOW REPORT
    // Rely on the implicit move semantics - don't move the function's return value explicitly to allow compiler to do Move Elision
    TerminalAndLoggerSingleton::get()
            .printAndLog(ignoredPackageNames->generateReport())
            .printAndLog(locallyInstalledPackages->generateReport())
            .printAndLog(matchFinderWithPackageFilesRelatedToPackages->generateReport());

    // MOVE PACKAGE FILES TO SEPARATE DIRECTORY
    auto installationPackageFilesMover = std::make_unique<MoverOfInstallationPackageFiles>(
            *matchFinderWithPackageFilesRelatedToPackages,
            *locallyInstalledPackages);
//    auto installationPackageFilesMover = std::make_unique<MoverOfInstallationPackageFiles>(
//            *locallyInstalledPackages,
//            *matchFinderWithPackageFilesRelatedToPackages,
//            directoryForMovedInstallationPackageFiles);

    // TODO restore 'root' permissions for symlink to pikaur system cache directory
    //    "/var/cache/pikaur"
    installationPackageFilesMover->moveChosenInstallationPackageFilesToSeparateDir();

    std::stringstream message{};
    message << "The log file for this cleanup session had been saved to:\n"
            << TerminalAndLoggerSingleton::get().getLogFilePath()
            << "\n";
    TerminalAndLoggerSingleton::get().printAndLog(message.str());

    return 0;
}
