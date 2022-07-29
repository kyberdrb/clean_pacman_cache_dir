//
// Created by laptop on 6/20/22.
//

#pragma once

#include "MatchFinderForPackageFilesToLocallyInstalledPackages.h"

class MoverOfInstallationPackageFiles {
public:
    explicit MoverOfInstallationPackageFiles(
            const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
            const LocallyInstalledPackages& locallyInstalledPackages);

    void moveChosenInstallationPackageFilesToSeparateDir(bool dryRun) const;

private:
    const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages;
    const LocallyInstalledPackages& locallyInstalledPackages;

    // TODO share one copy of 'pacmanCacheDir'
    // TODO rename member var 'pacmanCacheDir' to 'dirForInstalltionPackageFilesForDeletion'/'installationPackageFilesDeletionCandidatesDirectory'
    const std::string pacmanCacheDir = "/var/cache/pacman/pkg/";
};
