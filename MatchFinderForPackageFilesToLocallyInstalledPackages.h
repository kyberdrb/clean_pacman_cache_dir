//
// Created by laptop on 6/19/22.
//

#pragma once

#include "LocallyInstalledPackages.h"
#include "SimpleInstallationPackageFile.h"

class MatchFinderForPackageFilesToLocallyInstalledPackages {
public:
    explicit MatchFinderForPackageFilesToLocallyInstalledPackages(const LocallyInstalledPackages& locallyInstalledPackages);

    std::string generateReport() const;

    // TODO move function to separate class 'InstallationPackageFileMover' and use it in the client instead of here
    void moveInstallationPackageFilesForPackagesWithDifferenVersions() const;

private:
    const LocallyInstalledPackages& locallyInstalledPackages;
    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;

    // TODO maybe move this member variable to class 'LocallyInstalledPackages'?
    std::set<std::reference_wrapper<LocallyInstalledPackage>> packagesWithInstallationPackageFilesForDifferentVersions;

    void relatePackageFilesToLocallyInstalledPackages();
};
