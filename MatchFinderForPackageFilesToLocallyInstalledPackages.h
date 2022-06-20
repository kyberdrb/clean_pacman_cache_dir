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

    // TODO move function to separate class 'InstallationPackageFileMover' and use it in the client instead of here
    void moveChosenInstallationPackageFiles() const;

private:
    LocallyInstalledPackages& locallyInstalledPackages;
    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;

    void relatePackageFilesToLocallyInstalledPackages();
};
