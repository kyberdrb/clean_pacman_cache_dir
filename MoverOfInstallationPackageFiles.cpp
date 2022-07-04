//
// Created by laptop on 6/20/22.
//

#include "MoverOfInstallationPackageFiles.h"

#include <filesystem>

MoverOfInstallationPackageFiles::MoverOfInstallationPackageFiles(
        const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
        const LocallyInstalledPackages& locallyInstalledPackages)
:
    packageFilesRelatedToLocallyInstalledPackages(packageFilesRelatedToLocallyInstalledPackages),
    locallyInstalledPackages(locallyInstalledPackages)
{}

void MoverOfInstallationPackageFiles::moveChosenInstallationPackageFilesToSeparateDir() const {
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "MOVING PACKAGES\n\n";

    std::string pathToDuplicateFilesDirectoryAsText =
            this->pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";

    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    const auto directoryForInstallationPackageFilesForDeletion = std::make_unique<AbsolutePath>(pathToDuplicateFilesDirectoryAsText);

    locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
    packageFilesRelatedToLocallyInstalledPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
}
