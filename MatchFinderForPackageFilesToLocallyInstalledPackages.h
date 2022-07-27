//
// Created by laptop on 6/19/22.
//

#pragma once

#include "LocallyInstalledPackages.h"
#include "SimpleInstallationPackageFile.h"

class MatchFinderForPackageFilesToLocallyInstalledPackages {
public:
    explicit MatchFinderForPackageFilesToLocallyInstalledPackages(LocallyInstalledPackages& locallyInstalledPackages);

    std::string generateReport() const;

    void moveChosenInstallationPackageFiles(const AbsolutePath& directoryForInstallationPackageFilesForDeletion) const;

private:
    LocallyInstalledPackages& locallyInstalledPackages;

    // TODO share one copy of 'pacmanCacheDir'
    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";

    // TODO iterate through multiple directories with installation package files, not only within the default pacman's cache dir "/var/cache/pacman/pkg"
    //  but also in pikaur cache directories:
    //    - "/home/laptop/.cache/pikaur/pkg/"
    //    - "/var/cache/pikaur"

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;

    void relatePackageFilesToLocallyInstalledPackages();
    void relatePackageFilesToLocallyInstalledPackagesForDirectory(const AbsolutePath& directoryWithInstallationPackageFiles);
};
