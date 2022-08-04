//
// Created by laptop on 6/20/22.
//

#pragma once

#include "MatchFinderForPackageFilesToLocallyInstalledPackages.h"

class MoverOfInstallationPackageFiles {
public:
    MoverOfInstallationPackageFiles(
            const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
            const LocallyInstalledPackages& locallyInstalledPackages);

    void moveChosenInstallationPackageFilesToSeparateDir() const;

private:
    const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToPackages;
    const LocallyInstalledPackages& locallyInstalledPackages;
};
