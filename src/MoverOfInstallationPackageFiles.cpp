//
// Created by laptop on 6/20/22.
//

#include "MoverOfInstallationPackageFiles.h"

#include "Paths.h"
#include "TerminalAndLoggerSingleton.h"

#include <filesystem>

MoverOfInstallationPackageFiles::MoverOfInstallationPackageFiles(
        const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
        const LocallyInstalledPackages& locallyInstalledPackages)
:
        packageFilesRelatedToPackages(packageFilesRelatedToLocallyInstalledPackages),
        locallyInstalledPackages(locallyInstalledPackages)
{}

void MoverOfInstallationPackageFiles::moveChosenInstallationPackageFilesToSeparateDir() const {
    std::stringstream message;

    message
        << "\n"
        << "===============================================\n\n"
        << "MOVING PACKAGES\n\n";

    TerminalAndLoggerSingleton::get().printAndLog(message.str());

    std::filesystem::create_directories(Paths::get().getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText());

    const auto directoryForInstallationPackageFilesForDeletion =
            std::make_unique<AbsolutePath>(Paths::get().getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText());

    this->locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
    this->packageFilesRelatedToPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
    this->packageFilesRelatedToPackages.cleanUpOtherFilesInPikaurCacheDirs(*directoryForInstallationPackageFilesForDeletion);
}
