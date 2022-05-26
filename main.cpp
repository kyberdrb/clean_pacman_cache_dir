#include "IgnoredPackageName.h"
#include "Package.h"
//#include "PackageComparator.h"

#include "alpm.h"
#include "alpm_list.h"
#include "SimpleInstallationPackageFile.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>

//#include <cassert>

int main() {
    // FIND IGNORED PACKAGES PART - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES

    // 'alpm_option_get_ignorepkgs' to retrieve the list of ignored packages from pacman's config doesn't work. Parsing '/etc/pacman.conf' manually
    //alpm_list_t* listOfIgnoredPackages = alpm_option_get_ignorepkgs(handle);

    std::ifstream pacmanConfigFile;

    // TODO parametrize with argument (maybe use getopt?) - if parameter empty, then use default one + check whether the pacman configuration file in the default path actually exists; otherwise exit?/ask user whether to terminate or continue, because the configuration file is used to determine ignored packages in order to exclude them from deletion
    pacmanConfigFile.open("/etc/pacman.conf");

    std::string lineWithIgnoredPackages;
    std::smatch match;
    std::regex regexForIgnoredPackagesInPacmanConfigFile("^IgnorePkg = ");

    while (std::getline(pacmanConfigFile, lineWithIgnoredPackages)) {
        bool doesTheLineContainIgnoredPackages = std::regex_search(lineWithIgnoredPackages, match, regexForIgnoredPackagesInPacmanConfigFile);
        if (doesTheLineContainIgnoredPackages) {
            break;
        }
    }

    // TODO OPTIONAL (assuming no leading spaces/tabs) remove leading and ending blank characters
    // TODO OPTIONAL (assuming no ending spaces/tabs; only one space delimiting [separating] each package filename) replace multiple spaces or tabs with one space

    // build a list of ignored packages

    auto lineWithIgnoredPackagesWithoutOptionPrefix = regex_replace(lineWithIgnoredPackages, regexForIgnoredPackagesInPacmanConfigFile, "");
    std::stringstream ignoredPackagesAsStream{lineWithIgnoredPackagesWithoutOptionPrefix};
    std::string ignoredPackageNameAsText{};
    char delimiterForIgnoredPackages = ' ';

    std::vector<std::unique_ptr<IgnoredPackageName>> ignoredPackageNames;

    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPackages)) {
        auto ignoredPackageName = std::make_unique<IgnoredPackageName>(std::move(ignoredPackageNameAsText));
        ignoredPackageNames.emplace_back(std::move(ignoredPackageName));
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF IGNORED PACKAGES\n\n";

    std::cout << "Found " << ignoredPackageNames.size() << " ignored packages\n\n";

    for (const auto& ignoredPackageName : ignoredPackageNames) {
        std::cout << *ignoredPackageName  << "\n";
    }

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES

    // Assuming that each package has only one locally installed version with possibly multiple related package files for
    //  multiple versions that the Package will remember within itself
    //  and not as a value for the Package key.
    // "Some sets store object that embed their own keys, that is to say that such objects have a subpart that is to be considered as
    //  a key, like an ID for example, while the object itself is to be considered as a value."
    //   - https://www.fluentcpp.com/2017/06/09/search-set-another-type-key/
    //  Therefore a 'set' and
    //   - not a 'multiset' [the embedded key - package name - is unique - only one package name in all different versions of it
    //     than the local one],
    //   - not a 'map' [the values are related and contained in the key itself] and
    //   - not a 'multimap' [the key - package name - is unique - a filesystem feature: each file in a directory has a unique name]
    std::set<std::unique_ptr<Package>> installedPackages{};
//    std::set<std::unique_ptr<Package>, PackageComparator> installedPackages{};
//    std::set<std::unique_ptr<Package>, std::greater<std::unique_ptr<Package>>> installedPackages{};

    // Transparent functor 'std::greater' didn't work: no element was found
//    std::set<std::unique_ptr<Package>, std::greater<>> installedPackages{};

    alpm_errno_t* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);
    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageNameAsText = alpm_pkg_get_name(alpm_pkg);
        std::string locallyInstalledVersionAsText = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);

        bool isIgnored = false;
        auto ignoredPackageNameCandidate = std::make_unique<IgnoredPackageName>(packageNameAsText);

        // For debugging purposes - if the argument is passed by value to a function, which accepts the argument as a value
        //  the argument is __copied__ from the calling to the receiving function
//        assert(packageNameAsText != "");

        if( std::find(ignoredPackageNames.begin(), ignoredPackageNames.end(), ignoredPackageNameCandidate) != ignoredPackageNames.end() ) {
            isIgnored = true;
        }

        auto packageName = std::make_unique<PackageName>(std::move(packageNameAsText));

        // For debugging purposes - if the argument is passed by value with 'std::move' to a function, which accepts the argument as a value
        //  the argument is __moved__ from the calling to the receiving function
//        assert(packageNameAsText == "");

        auto locallyInstalledVersion = std::make_unique<PackageVersion>(std::move(locallyInstalledVersionAsText));
        auto pkg = std::make_unique<Package>(std::move(packageName), std::move(locallyInstalledVersion), architecture, isIgnored);

        installedPackages.emplace(std::move(pkg));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;

    // FIND PACKAGE FILES IN DIFFERENT PACKAGE VERSIONS FOR SAME PACKAGE,
    //  PARTIALLY DOWNLOADED PACKAGE FILES AND
    //  PACKAGE FILES FOR MISSING PACKAGES

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partlyDownloadedPackageFiles;

    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";
    std::filesystem::path aPath {pacmanCacheDir};

    for (const auto& packageFile : std::filesystem::directory_iterator(aPath)) {
        const auto& packageFileExtension = packageFile.path().extension().string();
        const auto& packageAbsolutePathAsText = packageFile.path().string();
        const auto& packageFilenameAsText = packageFile.path().filename().string();

        if (packageFileExtension == ".part") {
            auto partlyDownloadedPackageFile= std::make_unique<SimpleInstallationPackageFile>(packageAbsolutePathAsText, packageFilenameAsText);
            partlyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
            continue;
        }

        // For debugging purposes
//        if (packageFilenameAsText == "libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst") {
//            std::cout << "Here we go..." << "\n";
//        }

        if (packageFile.is_regular_file()) {
            // strip the extensions and architecture from package filename leaving only package name and package version
            //  I couldn't find out how to do first-match or non-greedy replace with regex_replace,
            //  so I'm writing my own algorithm
            auto pkgFilenameAsText = packageFilenameAsText;
            std::reverse(pkgFilenameAsText.begin(), pkgFilenameAsText.end());
            std::stringstream packageNameAndVersionReversed{};
            char delimiter = '-';
            bool stillSearchingForFirstDelimiterOccurence = true;

            //find the first dash '-' in the reversed filename and append everything after
            for (auto character : pkgFilenameAsText) {
                if (character != delimiter && stillSearchingForFirstDelimiterOccurence) {
                    continue;
                }

                stillSearchingForFirstDelimiterOccurence = false;
                packageNameAndVersionReversed << character;
            }

            auto packageNameAndVersion = packageNameAndVersionReversed.str();
            std::reverse(packageNameAndVersion.begin(), packageNameAndVersion.end());
            packageNameAndVersion.pop_back();

            std::string inferredPackageNameAsText = packageNameAndVersion;
            auto packageName = std::make_unique<PackageName>(std::move(inferredPackageNameAsText));
            auto packageWithInferredName = std::make_unique<Package>(std::move(packageName));

            while ( packageWithInferredName->hasStillSomethingInPackageName() ) {
                // search for the matching package element in the 'installedPackages' by 'packageWithInferredName'
                auto iteratorPointingToMatchingPackage = installedPackages.find(packageWithInferredName);

                // For debugging purposes - because the gdb debugger in CLion 2022.1 produces an error when
                //  trying to show the values for STL containers and smartpointer instances.
                //  Instead, it shows an error message saying "Cannot instantiate printer for default visualizer"
//                std::cout << *packageWithInferredName << "\n";

                // if key was NOT found, strip the coumpound package key by one character - or word  from the end and perform lookup again
                bool packageWithInferredNameIsMissing = iteratorPointingToMatchingPackage == installedPackages.end();
                if (packageWithInferredNameIsMissing) {
                    packageWithInferredName->getNextInferredPackageNameCandidate();
                    continue;
                }

                // For debugging purposes
//                assert(iteratorPointingToMatchingPackage->get()->getName().string() == packageWithInferredName->getName());

                // if the key WAS found,
                //  - infer the package version from the compound package name and version,
                //  - create a package file with filename, absolute path and package version
                //     and add it to the matching locally installed package
                //  - break out of the loop
                auto startingPositionForPackageVersion = packageWithInferredName->getStartingPositionForPackageVersion();
                auto inferredPackageVersionAsText = packageNameAndVersion.substr(startingPositionForPackageVersion);
                auto inferredPackageVersion = std::make_unique<PackageVersion>(inferredPackageVersionAsText);

                auto packageRelatedFile = std::make_unique<ExtendedInstallationPackageFile>(
                        std::move(packageFilenameAsText),
                        packageAbsolutePathAsText,
                        iteratorPointingToMatchingPackage->get()->getName(),
                        std::move(inferredPackageVersion));

                iteratorPointingToMatchingPackage->get()->addPackageFileToDeletionCandidates(std::move(packageRelatedFile));
                break;
            }

            bool hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage = packageWithInferredName->isPackageNameEmpty();
            if (hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage) {
                auto packageFileForMissingPackage = std::make_unique<SimpleInstallationPackageFile>(packageAbsolutePathAsText, packageFilenameAsText);
                packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF ALL INSTALLED PACKAGES WITH RELATED PACKAGE FILES FOR DIFFERENT VERSIONS (IF ANY)\n\n";

    std::cout << "Found " << installedPackages.size() << " installed packages\n\n";

    for (const auto& package : installedPackages) {
        std::cout << *package << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF INSTALLED PACKAGES THAT HAVE AT LEAST ONE RELATED INSTALLATION PACKAGE FILE FOR DIFFERENT VERSION THAN THE LOCALLY INSTALLED ONE\n\n";

    uint_fast16_t numberOfPackagesWithInstallationPackageFilesForOtherVersions = 0;
    uint_fast16_t numberOfInstallationPackageFilesForOtherVersions = 0;

    for (const auto& package : installedPackages) {
        uint_fast16_t numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage =
                package->getNumberOfInstallationPackageFilesForDifferentVersions();

        if (numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage > 0) {
            std::cout << *package << "\n";

            numberOfPackagesWithInstallationPackageFilesForOtherVersions++;
            numberOfInstallationPackageFilesForOtherVersions += numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage;
        }
    }

    std::cout << "\n";
    std::cout << "Found " << numberOfPackagesWithInstallationPackageFilesForOtherVersions
            << " installed packages with installation package files for other than locally installed version\n";

    std::cout << "Found " << numberOfInstallationPackageFilesForOtherVersions
            << " installation package files for different version than the locally installed\n\n";

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PARTLY DOWNLOADED PACKAGE FILES\n\n";

    std::cout << "Found " << partlyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : partlyDownloadedPackageFiles) {
        std::cout << *partlyDownloadedPackageFile << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGE FILES RELATED TO MISSING PACKAGES\n\n";

    std::cout << "Found " << packageFilesRelatedToMissingPackages.size() << " package files related to missing packages\n\n";

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        std::cout << *packageFilesRelatedToMissingPackage << "\n";
    }

    // MOVING PACKAGE FILES TO SEPARATE DIRECTORY PART

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "MOVING PACKAGES\n\n";

    std::string pathToDuplicateFilesDirectoryAsText =
            pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";
    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    for (const auto& installedPackage : installedPackages) {
        installedPackage->movePackageFilesForDifferentVersionsToSeparateDir(pathToDuplicateFilesDirectoryAsText);
    }

    for (const auto& partlyDownloadedPackageFile : partlyDownloadedPackageFiles) {
            const std::string& from = partlyDownloadedPackageFile->getAbsolutePath();
            const std::string& to = pathToDuplicateFilesDirectoryAsText +
                    partlyDownloadedPackageFile->getFilename(); // TODO 'getFilename()' will be always empty - pass by value with 'std::move'
                                                                // or deduce the filename in 'SimpleInstallationPackageFile::getFilename()' function
            std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        const std::string& from = packageFilesRelatedToMissingPackage->getAbsolutePath();
        const std::string& to = pathToDuplicateFilesDirectoryAsText +
                packageFilesRelatedToMissingPackage->getFilename();
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    // TODO completely clean all files ** only ** within the pikaur cache directory `"${HOME}/.cache/pikaur/"`

    // TODO completely clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/build/"`

    // TODO ** selectively ** clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/pkg/"

    return 0;
}
