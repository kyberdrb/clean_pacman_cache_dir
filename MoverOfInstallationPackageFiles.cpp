//
// Created by laptop on 6/20/22.
//

#include "MoverOfInstallationPackageFiles.h"
#include "TerminalSingleton.h"

#include <filesystem>

MoverOfInstallationPackageFiles::MoverOfInstallationPackageFiles(
        const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
        const LocallyInstalledPackages& locallyInstalledPackages)
:
    packageFilesRelatedToLocallyInstalledPackages(packageFilesRelatedToLocallyInstalledPackages),
    locallyInstalledPackages(locallyInstalledPackages)
{}

void MoverOfInstallationPackageFiles::moveChosenInstallationPackageFilesToSeparateDir(bool dryRun) const {
    std::stringstream message;

    message
        << "\n"
        << "===============================================\n\n"
        << "MOVING PACKAGES\n\n";

    TerminalSingleton::get().printText(message.str());

    std::string pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText =
            this->pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";

    std::filesystem::create_directories(pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText);

    const auto directoryForInstallationPackageFilesForDeletion =
            std::make_unique<AbsolutePath>(pathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText);

    if (dryRun) {
        return;
    }

    locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
    packageFilesRelatedToLocallyInstalledPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
}
