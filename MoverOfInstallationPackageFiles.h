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

    void moveChosenInstallationPackageFilesToSeparateDir() const;

private:
    const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages;
    const LocallyInstalledPackages& locallyInstalledPackages;

    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";
};
