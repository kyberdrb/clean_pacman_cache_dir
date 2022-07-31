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
    void cleanUpOtherFilesInPikaurCacheDirs(const AbsolutePath& destinationDirectory) const;

private:
    LocallyInstalledPackages& locallyInstalledPackages;

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;

    std::string currentUserHomeDir;

    void relateInstallationPackageFilesToLocallyInstalledPackagesForAllCacheDirs();
    void relatePackageFilesToLocallyInstalledPackagesForDirectory(const AbsolutePath& directoryWithInstallationPackageFiles);

    std::string determinePikaurCacheDirUser();

    void moveOnlyFilesFromDir(const AbsolutePath& absolutePathToSourceDirectoryAsText, const AbsolutePath& absolutePathToDestinationDirectoryAsText) const;
    void moveEverythingFromDir(const AbsolutePath& absolutePathToSourceDirectoryAsText, const AbsolutePath& absolutePathToDestinationDirectoryAsText) const;
};
