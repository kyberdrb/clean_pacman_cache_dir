//
// Created by laptop on 6/20/22.
//

#include "MoverOfInstallationPackageFile.h"

MoverOfInstallationPackageFile::MoverOfInstallationPackageFile(
        const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
        const LocallyInstalledPackages& locallyInstalledPackages)
:
    packageFilesRelatedToLocallyInstalledPackages(packageFilesRelatedToLocallyInstalledPackages),
    locallyInstalledPackages(locallyInstalledPackages)
{}

void MoverOfInstallationPackageFile::moveChosenInstallationPackageFilesToSeparateDir() const {
    packageFilesRelatedToLocallyInstalledPackages.moveInstallationPackageFilesForPackagesWithDifferenVersions();
//    locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir();
}
