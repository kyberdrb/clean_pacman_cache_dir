//
// Created by laptop on 6/20/22.
//

#pragma once

#include "MatchFinderForPackageFilesToLocallyInstalledPackages.h"

class MoverOfInstallationPackageFile {
public:
    explicit MoverOfInstallationPackageFile(
            const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
            const LocallyInstalledPackages& locallyInstalledPackages);

    void moveChosenInstallationPackageFilesToSeparateDir() const;

private:
    const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages;
    const LocallyInstalledPackages& locallyInstalledPackages;
};
