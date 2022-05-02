//
// Created by laptop on 4/29/22.
//

#include "Packages.h"

#include "LocallyInstalledPackages.h"

#include "alpm.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

void Packages::cleanCachedFilesOfPackageManagers() {
    // TODO parse options for 'ignoredPackageNames'
    //  - maybe with 'getopt' when custom pacman's configuration file is entered?
    //  - encapsulate the options in a separate class e.g. 'UserArguments' or 'Options'?

    auto ignoredPackageNames = std::make_unique<IgnoredPackageNames>();
    this->findIgnoredPackageNames();

    std::cout << ignoredPackageNames->generateReport(); // overload stream output operator
    this->printIgnoredPackageNames();

    auto locallyInstalledPackages = std::make_unique<LocallyInstalledPackages>(ignoredPackageNames);
    std::cout << locallyInstalledPackages->printInstalledPackages();
    this->findLocallyInstalledPackages();
    this->printInstalledPackages();

    // TODO 'installationPackageFilesRelatedToLocallyInstalledPackages' is the preferred variable name
    // TODO Find relations between locally installed packages and installation package files immediately in constructor; TODO Maybe add a second argument to constructor: a vector of paths on the filesystem to look up the related packages in? Include the pikaur dirs as well
    //auto finderOfInstallationPackageFilesForPackages = std::make_unique<FinderOfInstallationPackageFilesForPackages>(locallyInstalledPackages);
    //auto installationPackageFilesRelatedToLocallyInstalledPackages = std::make_unique<InstallationPackageFilesRelatedToLocallyInstalledPackages>(locallyInstalledPackages);
    this->relateInstallationPackageFilesToLocallyInstalledPackages();

    //installationPackageFilesRelatedToLocallyInstalledPackages->generateReport(); // overload stream output operator
    this->printInstalledPackages();
    this->printPartiallyDownloadedInstallationPackageFiles();
    this->printInstallationPackageFilesRelatedToMissingLocallyInstalledPackages();

    //auto installationPackageFilesMover = std::make_unique<InstallationPackageFilesMover>(finderOfInstallationPackageFilesForPackages); // TODO Move installation packages files to separate dir immediately in constructor
    this->moveInstallationPackageFilesToSeparateDirForDeletion();

    this->deleteAllCachedFilesForPikaur();  // TODO or run the algorithm again for pikaur cache subdirs? Maybe see TODOs for 'installationPackageFilesRelatedToLocallyInstalledPackages' in order to make the pikaur cache subdirs cleanup more considerate by keeping the files and dirs related to packages with matching name and version to locally installed package
}

void Packages::findIgnoredPackageNames() {
    std::ifstream pacmanConfigFile;

    //  'alpm_option_get_ignorepkgs' to retrieve the list of ignored packages from pacman's config doesn't work. Parsing '/etc/pacman.conf' manually
    //    alpm_list_t* listOfIgnoredPackages = alpm_option_get_ignorepkgs(handle);

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
    char delimiterForIgnoredPakcages = ' ';

    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPakcages)) {
        this->ignoredPackageNames.push_back(std::make_unique<PackageName>(ignoredPackageNameAsText) );
    }
}

void Packages::printIgnoredPackageNames() const {
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF IGNORED PACKAGES\n\n";

    std::cout << "Found " << this->ignoredPackageNames.size() << " ignored packages\n\n";

    for (const auto& ignoredPackageName : this->ignoredPackageNames) {
        std::cout << *ignoredPackageName  << "\n";
    }
}

void Packages::findLocallyInstalledPackages() {
    alpm_errno_t* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);
    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageNameAsText = alpm_pkg_get_name(alpm_pkg);
        std::string locallyInstalledVersion = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);

        // For debugging purposes
//        if (packageNameAsText == "clion") {
//            std::cout << "Here we go..." << "\n";
//        }

        auto packageName = std::make_unique<PackageName>(packageNameAsText);
        bool isIgnored = false;

        bool isPackageNameCandidateMatchingToExistingPackageName =
            std::find_if(
                this->ignoredPackageNames.begin(),
                this->ignoredPackageNames.end(),
                [&packageName](const std::unique_ptr<PackageName>& packageNameCandidate) {
                    return *packageName == *packageNameCandidate;
                }
            ) != this->ignoredPackageNames.end();

        if (isPackageNameCandidateMatchingToExistingPackageName)
        {
            isIgnored = true;
        }

        auto pkg = std::make_unique<Package>(packageNameAsText, locallyInstalledVersion, architecture, isIgnored);

        this->installedPackages.emplace(std::move(pkg));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;
}

void Packages::relateInstallationPackageFilesToLocallyInstalledPackages() {
    std::filesystem::path pacmanCacheDir {Packages::PACMAN_CACHE_DIR_PATH};

    for (const auto& packageFile : std::filesystem::directory_iterator(pacmanCacheDir)) {
        const auto& packageFilenameAsText = packageFile.path().filename().string();
        const auto& packageAbsolutePathAsText = packageFile.path().string();
        const auto& packageFileExtension = packageFile.path().extension().string();

        if (packageFileExtension == ".part") {
            auto partlyDownloadedPackageFile= std::make_unique<PackageFile>(packageAbsolutePathAsText);
            this->partlyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
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
            auto packageWithInferredName = std::make_unique<Package>(std::move(inferredPackageNameAsText));
            bool wasInferredPackageRefferingToMissingPackage = false;

            while ( packageWithInferredName->hasStillSomethingInPackageName() ) {
                // search for the matching package element in the 'installedPackages' by 'packageWithInferredName'
                auto matchingPackage = this->installedPackages.find(packageWithInferredName);

                // if key was NOT found, generate a new key candidate and perform lookup again
                bool packageWithInferredNameIsMissing = matchingPackage == this->installedPackages.end();
                if (packageWithInferredNameIsMissing) {
                    packageWithInferredName->getNextInferredPackageNameCandidate();
                    continue;
                }

                // if the key WAS found,
                //  - infer the package version from the compound package name and version,
                //  - create a package file with filename, absolute path and package version and add it to the matching locally installed package
                //  - break out of the loop
                auto startingPositionForPackageVersion = packageWithInferredName->getStartingPositionForPackageVersion();
                auto inferredPackageVersionAsText = packageNameAndVersion.substr(startingPositionForPackageVersion);
                wasInferredPackageRefferingToMissingPackage = inferredPackageVersionAsText.empty();

                std::unique_ptr<PackageName> inferredPackageName = std::move(packageWithInferredName->moveNameHere());

                auto packageWithInferredNameAndVersion = std::make_unique<PackageWithInferredNameAndVersion>(
                        std::move(inferredPackageName),
                        std::move(inferredPackageVersionAsText));

                auto packageRelatedFile = std::make_unique<PackageFile>(
                        packageFilenameAsText,
                        packageAbsolutePathAsText,
                        std::move(packageWithInferredNameAndVersion) );

                matchingPackage->get()->addPackageFileToDeletionCandidates(std::move(packageRelatedFile));
                break;
            }

            // If the lookup didn't find matching package name for all of the package filename candidates
            //  the package file is likely to be referring to a missing or uninstalled package
            if (wasInferredPackageRefferingToMissingPackage) {
                auto packageFileForMissingPackage = std::make_unique<PackageFile>(packageAbsolutePathAsText);
                this->packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }
}

void Packages::printInstalledPackages() const {
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF INSTALLED PACKAGES WITH RELATED PACKAGE FILES FOR DIFFERENT VERSIONS\n\n";

    std::cout << "Found " << this->installedPackages.size() << " installed packages\n\n";

    for (const auto& package : this->installedPackages) {
        std::cout << *package << "\n";
    }
}

void Packages::printPartiallyDownloadedInstallationPackageFiles() const {
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PARTLY DOWNLOADED PACKAGE FILES\n\n";

    std::cout << "Found " << this->partlyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : this->partlyDownloadedPackageFiles) {
        std::cout << *partlyDownloadedPackageFile << "\n";
    }
}

void Packages::printInstallationPackageFilesRelatedToMissingLocallyInstalledPackages() const {
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGE FILES RELATED TO MISSING PACKAGES\n\n";

    std::cout << "Found " << packageFilesRelatedToMissingPackages.size() << " package files related to missing packages\n\n";

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        std::cout << *packageFilesRelatedToMissingPackage << "\n";
    }
}

void Packages::moveInstallationPackageFilesToSeparateDirForDeletion() {
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "MOVING PACKAGES\n\n";

    bool endsPathToDirectoryWithForwardSlash = this->PACMAN_CACHE_DIR_PATH.at(this->PACMAN_CACHE_DIR_PATH.size() - 1) == '/';
    if (endsPathToDirectoryWithForwardSlash) {
        this->PACMAN_CACHE_DIR_PATH.pop_back();
    }

    std::string pathToDuplicateFilesDirectoryAsText =
            PACMAN_CACHE_DIR_PATH + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";
    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    for (const auto& installedPackage: this->installedPackages) {
        // TODO differentiate between installer package files for nonignored packages
        //  and ignored packages - move duplicate ignored packages to a separate directory
        //  'PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED_FOR_IGNORED_PACKAGES'
        installedPackage->movePackageFilesForDifferentVersionsToSeparateDir(pathToDuplicateFilesDirectoryAsText);
    }

    for (const auto& partlyDownloadedPackageFile: partlyDownloadedPackageFiles) {
        const std::string& from = partlyDownloadedPackageFile->getAbsolutePath();
        const std::string& to = pathToDuplicateFilesDirectoryAsText +
                                partlyDownloadedPackageFile->getFilename();
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
        std::filesystem::rename(from, to);
    }

    for (const auto& packageFilesRelatedToMissingPackage: packageFilesRelatedToMissingPackages) {
        const std::string& from = packageFilesRelatedToMissingPackage->getAbsolutePath();
        const std::string& to = pathToDuplicateFilesDirectoryAsText +
                                packageFilesRelatedToMissingPackage->getFilename();
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
        std::filesystem::rename(from, to);
    }
}

void Packages::deleteAllCachedFilesForPikaur() {
    std::vector<std::filesystem::directory_entry> pikaurNestedDirs;

    std::string pikaurCacheDir = "/var/cache/pikaur"; // TODO and also in '/home/laptop/.cache/pikaur' or '${HOME}/.cache/pikaur'
    std::filesystem::path pikaurCacheDirPath(pikaurCacheDir);

    for (auto& pikaurCacheEntry : std::filesystem::directory_iterator(pikaurCacheDirPath)) {
        if (pikaurCacheEntry.is_directory()) {
            pikaurNestedDirs.emplace_back(std::move(*const_cast<std::filesystem::directory_entry*>( &(pikaurCacheEntry)) ) );
        }
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST PIKAUR CACHE DIRS\n\n";

    for (auto& pikaurCacheNestedDir : pikaurNestedDirs) {
        std::cout << pikaurCacheNestedDir.path().string() << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "DELETE PIKAUR NESTED CACHE ENTRIES\n\n";

    for (auto& pikaurCacheNestedDir : pikaurNestedDirs) {
        std::filesystem::path pikaurCacheDirPath(pikaurCacheNestedDir.path().string());

        for (auto& pikaurNestedCacheEntry : std::filesystem::directory_iterator(pikaurCacheDirPath)) {
            std::cout << "Deleting file:" << "\n";
            std::cout << pikaurNestedCacheEntry.path().string() << "\n\n";
            std::filesystem::remove_all(pikaurNestedCacheEntry);
        }
    }
}