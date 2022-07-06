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

void MoverOfInstallationPackageFiles::moveChosenInstallationPackageFilesToSeparateDir() const {
    std::stringstream message;

    message
        << "\n"
        << "===============================================\n\n"
        << "MOVING PACKAGES\n\n";

    TerminalSingleton::get().printText(message.str());

    std::string pathToDuplicateFilesDirectoryAsText =
            this->pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";

    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    const auto directoryForInstallationPackageFilesForDeletion = std::make_unique<AbsolutePath>(pathToDuplicateFilesDirectoryAsText);

    locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
    packageFilesRelatedToLocallyInstalledPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
}
