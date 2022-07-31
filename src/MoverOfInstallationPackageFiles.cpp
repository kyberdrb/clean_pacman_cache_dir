//
// Created by laptop on 6/20/22.
//

#include "MoverOfInstallationPackageFiles.h"
#include "TerminalAndLoggerSingleton.h"

#include <filesystem>

MoverOfInstallationPackageFiles::MoverOfInstallationPackageFiles(
        const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
        const LocallyInstalledPackages& locallyInstalledPackages)
:
    packageFilesRelatedToLocallyInstalledPackages(packageFilesRelatedToLocallyInstalledPackages),
    locallyInstalledPackages(locallyInstalledPackages)
{}

void MoverOfInstallationPackageFiles::moveChosenInstallationPackageFilesToSeparateDir() const {
    std::stringstream message;

    message
        << "\n"
        << "===============================================\n\n"
        << "MOVING PACKAGES\n\n";

    TerminalAndLoggerSingleton::get().printAndLog(message.str());

    std::string pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText =
            this->pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";

    std::filesystem::create_directories(pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText);

    const auto directoryForInstallationPackageFilesForDeletion =
            std::make_unique<AbsolutePath>(pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText);

    locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
    packageFilesRelatedToLocallyInstalledPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
    packageFilesRelatedToLocallyInstalledPackages.cleanUpOtherFilesInPikaurCacheDirs(*directoryForInstallationPackageFilesForDeletion);
}
