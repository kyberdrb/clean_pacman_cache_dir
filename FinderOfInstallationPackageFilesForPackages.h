//
// Created by laptop on 5/3/22.
//

#pragma once

#include <ostream>
#include "LocallyInstalledPackages.h"

class FinderOfInstallationPackageFilesForPackages {
public:
    explicit FinderOfInstallationPackageFilesForPackages(std::unique_ptr<LocallyInstalledPackages>& locallyInstalledPackages);
    //explicit FinderOfInstallationPackageFilesForPackages(LocallyInstalledPackages& locallyInstalledPackages);

    std::string generateReport() const;

private:
    const std::string PACMAN_CACHE_DIR_PATH = "/var/cache/pacman/pkg";

    std::unique_ptr<LocallyInstalledPackages>& locallyInstalledPackages;
    //LocallyInstalledPackages& locallyInstalledPackages;

    std::set<std::unique_ptr<PackageFile>> partlyDownloadedPackageFiles;
    //std::unique_ptr<PartlyDownloadedPackageFiles> partlyDownloadedPackageFiles; // with 'std::set<std::unique_ptr<PackageFile>> partlyDownloadedPackageFiles;'
    //PartlyDownloadedPackageFiles& partlyDownloadedPackageFiles; // with 'std::set<std::unique_ptr<PackageFile>> partlyDownloadedPackageFiles;'

    std::set<std::unique_ptr<PackageFile>> packageFilesRelatedToMissingPackages;
    //std::unique_ptr<PackageFilesRelatedToMissingPackages> packageFilesRelatedToMissingPackages; // with 'std::set<std::unique_ptr<PackageFile>> packageFilesRelatedToMissingPackages;'
    //PackageFilesRelatedToMissingPackages& packageFilesRelatedToMissingPackages; // with 'std::set<std::unique_ptr<PackageFile>> packageFilesRelatedToMissingPackages;'

    void relateInstallationPackageFilesToLocallyInstalledPackages();

public:
    friend std::ostream& operator<<(std::ostream& os, const FinderOfInstallationPackageFilesForPackages& packages);
};
