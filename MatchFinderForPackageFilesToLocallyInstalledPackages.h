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

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;

    void relatePackageFilesToLocallyInstalledPackages();
    void relatePackageFilesToLocallyInstalledPackagesForDirectory(const AbsolutePath& directoryWithInstallationPackageFiles);
};
