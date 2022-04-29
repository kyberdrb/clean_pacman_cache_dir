//
// Created by laptop on 4/29/22.
//

#pragma once

#include "Package.h"

#include <set>
#include <string>
#include <vector>

class Packages {
public:
    Packages() = default;

    void cleanCachedFilesOfPackageManagers();

private:
    std::string PACMAN_CACHE_DIR_PATH = "/var/cache/pacman/pkg";

    std::vector<std::string> ignoredPackageNamesInTextFormat;
    std::set<std::unique_ptr<Package>> installedPackages;
    std::set<std::unique_ptr<PackageFile>> partlyDownloadedPackageFiles;
    std::set<std::unique_ptr<PackageFile>> packageFilesRelatedToMissingPackages;

    // FIND IGNORED PACKAGES PART - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES
    void findIgnoredPackageNames();
    void printIgnoredPackageNames() const;

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES
    //  Assuming that each package has only one locally installed version with possibly multiple related package files for multiple versions that the Package will remember with itself
    //   and not as a value for the Package key,
    //   therefore a 'set' and
    //    - not a 'multiset' [only one package name with multiple possible versions of it],
    //    - not a 'map' [the values are related and contained in the key itself] and
    //    - not a 'multimap' [the key - package name - is unique - a filesystem feature: each file in a directory has a unique name]
    void findLocallyInstalledPackages();

    // FIND PACKAGE FILES IN DIFFERENT PACKAGE VERSIONS FOR SAME PACKAGE,
    //  PARTIALLY DOWNLOADED PACKAGE FILES AND
    //  PACKAGE FILES FOR MISSING PACKAGES
    void relateInstallationPackageFilesToLocallyInstalledPackages();
    void printInstalledPackages() const;
    void printPartiallyDownloadedInstallationPackageFiles() const;
    void printInstallationPackageFilesRelatedToMissingLocallyInstalledPackages() const;

    // MOVING PACKAGE FILES TO SEPARATE DIRECTORY
    void moveInstallationPackageFilesToSeparateDirForDeletion();

    void deleteAllCachedFilesForPikaur();
};
