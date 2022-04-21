#include "Architectures.h"
#include "Extensions.h"
#include "PackageName.h"

#include "Package.h"
#include "PackageFile.h"

#include "alpm.h"
#include "alpm_list.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <map>

#include <regex>

#include <cassert>

int main() {
    // FIND IGNORED PACKAGES PART - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES

    // 'alpm_option_get_ignorepkgs' to retrieve the list of ignored packages from pacman's config doesn't work. Parsing '/etc/pacman.conf' manually
    //alpm_list_t* listOfIgnoredPackages = alpm_option_get_ignorepkgs(handle);

    std::ifstream pacmanConfigFile;
    // TODO parametrize with argument (maybe use getopt?) - if parameter empty, then use default one + check whether the pacman configuration file in the default path actually exists; otherwise exit?/ask user whether to terminate or continue, because the configuration file is used to determine ignored packages in order to exclude them from deletion
    pacmanConfigFile.open("/etc/pacman.conf");

    std::map<std::unique_ptr<PackageName>, std::unique_ptr<Package>> ignoredPackages;

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

    std::stringstream ignoredPackagesAsStream{lineWithIgnoredPackages};
    std::string ignoredPackageNameAsText{};
    std::vector<std::string> ignoredPackageNamesInTextFormat{};

    char delimiterForIgnoredPakcages = ' ';
    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPakcages)) {
        ignoredPackageNamesInTextFormat.push_back(ignoredPackageNameAsText);
    }

    // ALPM PART

    alpm_errno_t* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);
    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    std::map<std::string, std::string> allPackagesInTextFormat;
    std::set<std::string> packagesToKeep;
    auto architectures = std::make_unique<Architectures>();

    std::map<std::unique_ptr<PackageName>, std::unique_ptr<Package>> locallyInstalledPackages;
    std::map<std::unique_ptr<PackageName>, std::unique_ptr<Package>> locallyInstalledSpecialPackages;

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        allPackagesInTextFormat.emplace(alpm_pkg_get_name(alpm_pkg), alpm_pkg_get_version(alpm_pkg));
        architectures->addArchitecture(alpm_pkg_get_arch(alpm_pkg));
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageName = alpm_pkg_get_name(alpm_pkg);
        std::string locallyInstalledVersion = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);

        auto pkgName = std::make_unique<PackageName>(packageName);
        auto pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);

        if(std::find(ignoredPackageNamesInTextFormat.begin(), ignoredPackageNamesInTextFormat.end(), packageName) != ignoredPackageNamesInTextFormat.end()) {
            ignoredPackages.emplace(std::move(pkgName), std::move(pkg));
            continue;
        }

        if (pkg->isSpecial()) {     // introducing new member variable 'bool isSpecialPackage' defaulting to 'false', and in constructor is set to 'true' if theconditions in the above TODO items are met
            locallyInstalledSpecialPackages.emplace(std::move(pkgName), std::move(pkg));
            continue;
        }

        locallyInstalledPackages.emplace(std::move(pkgName), std::move(pkg));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;

    std::cout << *architectures << "\n";

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF IGNORED PACKAGES IN MAP\n\n";

    std::cout << "Found " << ignoredPackages.size() << " ignored packages\n\n";

    for (const auto& [ignoredPackageName, package] : ignoredPackages) {
        std::cout << *ignoredPackageName << "\t" << *package << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF SPECIAL PACKAGES IN MAP\n\n";

    std::cout << "Found " << locallyInstalledSpecialPackages.size() << " ignored packages\n\n";

    for (const auto& [specialPackageName, package] : locallyInstalledSpecialPackages) {
        std::cout << *specialPackageName << "\t" << *package << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF LOCALLY INSTALLED PACKAGES IN MAP WITH CUSTOM-OBJECT KEYS\n\n";

    std::cout << "Found " << locallyInstalledPackages.size() << " installed packages\n";
    if (locallyInstalledPackages.size() <= allPackagesInTextFormat.size()) {
        std::cout << "Found " << ignoredPackages.size() << " ignored packages\n";
        std::cout << "Found " << locallyInstalledSpecialPackages.size() << " special packages\n";
        std::cout << "Found " << locallyInstalledPackages.size() + ignoredPackages.size() + locallyInstalledSpecialPackages.size() << " packages altogether\n\n";
        assert(locallyInstalledPackages.size() + ignoredPackages.size() + locallyInstalledSpecialPackages.size() == allPackagesInTextFormat.size());
    }

    for (const auto& [installedPackageName, package] : locallyInstalledPackages) {
        std::cout << *installedPackageName << "\t" << *package << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF LOCALLY INSTALLED PACKAGES\n\n";

    std::cout << "Found " << allPackagesInTextFormat.size() << " installed packages\n\n";

    for (const auto& [installedPackageName, installedPackageVersion] : allPackagesInTextFormat) {
        std::cout << installedPackageName << "\t" << installedPackageVersion << "\n";
    }

    // FILESYSTEM PART

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGE FILES\n\n";

    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";
    std::filesystem::path aPath {pacmanCacheDir};
    std::set<std::string> downloadedPackages;
    auto extensions = std::make_unique<Extensions>();
    std::set<std::string> packageFilesDesignatedForDeletion;

    for (const auto& packageRelatedFile : std::filesystem::directory_iterator(aPath)) {
        const auto& packageFilename = packageRelatedFile.path().filename().string();
        const auto& extension = packageRelatedFile.path().extension().string();
        if (packageRelatedFile.is_regular_file() ) {
            // for debugging purposes
//            std::cout << packageFilename << "\n";
            if (extension == ".part") {
                packageFilesDesignatedForDeletion.emplace(packageFilename);
                continue;
            }

            downloadedPackages.emplace(packageFilename);
            extensions->addExtension(extension);

            // REGEX REPLACE TO REPLACING THE TRAILING PART OF PACKAGE FILENAMES
            //  leaving only package filename and package version

            std::string pkgFilename = packageFilename;
            //std::cout << "Package filename:\n";
            //std::cout << pkgFilename << "\n";

            std::string architecturesRegexPatternAsText{};

            for (const auto& architecture : architectures->architectures) {
                architecturesRegexPatternAsText += architecture + '|';
            }

            architecturesRegexPatternAsText.pop_back();

            const std::string pattern = "-(" + architecturesRegexPatternAsText + ")\\.pkg\\..*";
            //std::cout << pattern << "\n";
            std::regex regularExpression(pattern);
            auto pkgNameAndVersion = regex_replace(pkgFilename, regularExpression, "");
            //std::cout << pkgNameAndVersion << "\n";

            std::stringstream pkgFilenameAsStream{pkgNameAndVersion};
            std::string token{};
            std::vector<std::string> tokens{};

            char delimiter = '-';
            while(getline(pkgFilenameAsStream, token, delimiter)) {
                tokens.push_back(token);
            }

            // for debugging purposes - pakcage with longer version
//            if (pkgNameAndVersion == "adobe-source-code-pro-fonts-2.038ro+1.058it+1.018var-1") {
//                auto p = static_cast<std::string*>(&pkgNameAndVersion);
//            }

            std::string packageName{};
            std::string packageVersion{};

            bool hasPackageNameMoreTokens = true;

            // assuming package filename format
            //  (\w.*-?)*(\d.*-?)*
            //  ˇˇˇˇˇˇˇˇˇ.........
            //  package  package
            //  filename     version

            for (const auto& token : tokens) {
                bool isFirstCharacterOfTokenLetter = !isdigit(token.at(0));

                if (isFirstCharacterOfTokenLetter && hasPackageNameMoreTokens) {
                    packageName += token + delimiter;
                    continue;
                }

                hasPackageNameMoreTokens = false;

                packageVersion += token + delimiter;
            }

            packageName.pop_back();
            packageVersion.pop_back();

            //std::cout << "Package filename:\t\t" << packageName << "\n";
            //std::cout << "Package version:\t" << packageVersion << "\n";

            // for debugging purposes - package with longer version
//            if (packageName == "adobe-source-code-pro-fonts") {
//                auto p = static_cast<std::string*>(&packageName);
//            }

            auto pkgName = std::make_unique<PackageName>(packageName);
            if (ignoredPackages.count(pkgName) == 1) {
                std::cout << "Package '" << packageName << "' is marked as ignored, i. e. listed in pacman's configuration file in 'IgnorePkg' entry" << "\n";
                continue;
            }

            // TODO verify that the extracted package filename and extracted package version matches the values already set for the Package instances in the locallyInstalledNonignoredPackages map
            pkgName = std::make_unique<PackageName>(packageName);

            if (locallyInstalledPackages.count(pkgName) == 0) {
                std::cout << "Package: '" << packageName << "' not found. The package is no longer present in the system, "
                                                            "or the package had been repackaged to another filename, "
                                                            "or the package had been uninstalled" << "\n";
                packageFilesDesignatedForDeletion.emplace(packageFilename);
                continue;
            }

            pkgName = std::make_unique<PackageName>(packageName);
            const std::string&& locallyInstalledVersion = locallyInstalledPackages.at(std::move(pkgName))->getLocallyInstalledVersion();
            if (packageVersion != locallyInstalledVersion) {    // possibly nullptr (key not found) and consequent crash on error
                // for debugging purposes
                //std::cout << "Package: '" << packageName << "' has different package file and locally installed version: " << "'" << packageVersion << "' and '" << locallyInstalledVersion << "'" << "\n";

                packageFilesDesignatedForDeletion.emplace(packageFilename);
            }
        }
    }

    std::cout << *extensions << "\n";

    // TODO check whether each package in locallyInstalledNonignoredPackages has at least one related package file
    //   if a locallyInstalledPackage has 0 related files
    //     iterate all files in the pacman's cache directory, compound a filename from the package member variables in format "packageName-packageVersion-packageArchitecture" and find all package files that begin like that

    // TODO check whether each package in locallyInstalledPackageWithSpecialNames has at least one related package file

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGES MARKED FOR DELETION\n\n";

    std::cout << "Found " << packageFilesDesignatedForDeletion.size() << " designated for deletion\n\n";

    for (const auto& packageForDeletion : packageFilesDesignatedForDeletion) {
        std::cout << packageForDeletion << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF LOCALLY DOWNLOADED PACKAGES\n\n";

    std::cout << "Found " << downloadedPackages.size() << " downloaded packages\n\n";

    for (const auto& downloadedPackageFilename : downloadedPackages) {
        std::cout << downloadedPackageFilename << "\n";
    }

    std::cout << "\n";
    std::cout << "Does '/var/cache/pacman/pkg/accounts-qml-module-0.7-4-x86_64.pkg.tar.zst' exist?" << "\n";
    std::cout << std::filesystem::exists("/var/cache/pacman/pkg/accounts-qml-module-0.7-4-x86_64.pkg.tar.zst");

    std::cout << "\n\n";
    std::cout << "Does '/var/cache/pacman/pkg/accounts-qml-module-0.7-4-x86_64.pkg.tar.zst.sig' exist?" << "\n";
    std::cout << std::filesystem::exists("/var/cache/pacman/pkg/accounts-qml-module-0.7-4-x86_64.pkg.tar.zst.sig");

    std::cout << "\n\n";
    std::cout << "Does '/var/cache/pacman/pkg/accountsservice-22.08.8-1-x86_64.pkg.tar.zst' exist?" << "\n";
    std::cout << std::filesystem::exists("/var/cache/pacman/pkg/accountsservice-22.08.8-1-x86_64.pkg.tar.zst");

    std::cout << "\n\n";
    std::cout << "Does '/var/cache/pacman/pkg/accountsservice-22.08.8-1-x86_64.pkg.tar.zst.sig' exist?" << "\n";
    std::cout << std::filesystem::exists("/var/cache/pacman/pkg/accountsservice-22.08.8-1-x86_64.pkg.tar.zst.sig");

    std::cout << "\n\n";
    std::cout << "===============================================\n\n";
    std::cout << "REGEX SUBSTITUTION TESTING\n\n";

    std::string pkgFilename = "ca-certificates-utils-20210603-1-any.pkg.tar.zst";
    std::cout << "Package filename:\n";
    std::cout << pkgFilename << "\n";

    std::string architecturesAsText{};

    for (const auto& architecture : architectures->architectures) {
        architecturesAsText += architecture + '|';
    }

    architecturesAsText.pop_back();

    const std::string pattern = "-(" + architecturesAsText + ")\\.pkg\\..*";
    std::cout << pattern << "\n";
    std::regex regularExpression(pattern);
    auto pkgNameAndVersion = regex_replace(pkgFilename, regularExpression, "");
    std::cout << pkgNameAndVersion << "\n";

    std::stringstream pkgFilenameAsStream{pkgNameAndVersion};
    std::string token{};
    std::vector<std::string> tokens{};

    char delimiter = '-';
    while(getline(pkgFilenameAsStream, token, delimiter))
    {
        tokens.push_back(token);
    }

    std::string packageName{};
    std::string packageVersion{};

    for (const auto& token : tokens) {
        if (!isdigit(token.at(0))) {
            packageName += token + delimiter;
            continue;
        }

        packageVersion += token + delimiter;
    }

    packageName.pop_back();
    packageVersion.pop_back();

    std::cout << "Package filename:\t\t" << packageName << "\n";
    std::cout << "Package version:\t" << packageVersion << "\n";

    std::cout << "\n";

    // TODO implement Algorithm 3

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGE FILES FOR ALGORITHM 3\n\n";

    std::map<std::unique_ptr<PackageFile>, std::unique_ptr<Package>> packageFilesWithPackages;
    std::set<std::string> packageFilesDeletionCandidates;

    for (const auto& packageFile : std::filesystem::directory_iterator(aPath)) {
        const auto& packageFilenameAsText = packageFile.path().filename().string();
        const auto& packageAbsolutePathAsText = packageFile.path().string();
        auto packageFilename = std::make_unique<PackageFile>(packageFilenameAsText, packageAbsolutePathAsText);

        if (packageFile.is_regular_file()) {
            auto package = std::make_unique<Package>("", "", "");
            packageFilesWithPackages.emplace(std::move(packageFilename), std::move(package));
        }
    }

    err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    handle = alpm_initialize("/", "/var/lib/pacman/", err);
    db_local = alpm_get_localdb(handle);
    listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    //listOfAllLocallyInstalledPackages = alpm_list_last(listOfAllLocallyInstalledPackages);

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);
        //listOfAllLocallyInstalledPackages = alpm_list_previous(listOfAllLocallyInstalledPackages);

        std::string packageName = alpm_pkg_get_name(alpm_pkg);
        std::string locallyInstalledVersion = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);
        auto pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);

        if (std::find(ignoredPackageNamesInTextFormat.begin(), ignoredPackageNamesInTextFormat.end(), packageName) != ignoredPackageNamesInTextFormat.end()) {
            auto partialPackageNamePrefixAsText = pkg->getName();
            auto partialPackageFilenamePrefix = std::make_unique<PackageFile>(partialPackageNamePrefixAsText);
            auto partiallyMatchedPackageFileElementByPrefix = packageFilesWithPackages.lower_bound(partialPackageFilenamePrefix);

            // TODO for ignored packages, build the partial filename only with packageName,
            //  then find ALL package files that begin with the name of the package
            //  and add/move the package as a Package value to the corresponding PackageFile keys (don't worry about the duplicate instances - we will handle them later by references)
//          For debugging purposes
//            if (packageName == "clion") {
//            if (packageName == "clion-cmake") {
//            if (packageName == "linux-lts-headers") {
//                std::cout << "Here we go..." << "\n";
//            }

            while (partiallyMatchedPackageFileElementByPrefix != std::end(packageFilesWithPackages) && partiallyMatchedPackageFileElementByPrefix->first->isPartiallyMatchingInPrefix(*partialPackageFilenamePrefix) == 0) {
                pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);  // Don't mind of duplicates
                partiallyMatchedPackageFileElementByPrefix->second = std::move(pkg);
                std::cout << *partiallyMatchedPackageFileElementByPrefix->first << "\t"<< *partiallyMatchedPackageFileElementByPrefix->second << "\n";
                ++partiallyMatchedPackageFileElementByPrefix;
            }
            continue;
        }

        auto partialPackageNamePrefixAsText = pkg->buildPartialPackageNamePrefix();
        auto partialPackageFilenamePrefix = std::make_unique<PackageFile>(partialPackageNamePrefixAsText);
        auto partiallyMatchedPackageFileElementByPrefix = packageFilesWithPackages.lower_bound(partialPackageFilenamePrefix);

        while (partiallyMatchedPackageFileElementByPrefix != std::end(packageFilesWithPackages) && partiallyMatchedPackageFileElementByPrefix->first->isPartiallyMatchingInPrefix(*partialPackageFilenamePrefix) == 0) {
            pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);  // Don't mind of duplicates
            partiallyMatchedPackageFileElementByPrefix->second = std::move(pkg);
            std::cout << *partiallyMatchedPackageFileElementByPrefix->first << "\t"<< *partiallyMatchedPackageFileElementByPrefix->second << "\n";
            ++partiallyMatchedPackageFileElementByPrefix;
        }
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;

    // report/verification which packages have 0 packageVersionsWithRelatedDownloadedFiles
    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGE FILES WITH ASSIGNED PACKAGES\n\n";

    std::cout << "Found " << packageFilesWithPackages.size() << " package files with packages\n\n";

    for (const auto& packageFileWithPackage : packageFilesWithPackages) {
        std::cout << *packageFileWithPackage.first << "\t";
        std::cout << *packageFileWithPackage.second << "\n";
    }

    err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    handle = alpm_initialize("/", "/var/lib/pacman/", err);
    db_local = alpm_get_localdb(handle);
    listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageName = alpm_pkg_get_name(alpm_pkg);
        std::string locallyInstalledVersion = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);
        auto pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);

        auto partialPackageNamePrefixAsText = pkg->getName();
        auto partialPackageFilenamePrefix = std::make_unique<PackageFile>(partialPackageNamePrefixAsText);
        auto partiallyMatchedPackageFileElementByPrefix = packageFilesWithPackages.lower_bound(partialPackageFilenamePrefix);
        bool hasPackageItsPackageFile = false;

        while (partiallyMatchedPackageFileElementByPrefix != std::end(packageFilesWithPackages) && partiallyMatchedPackageFileElementByPrefix->first->isPartiallyMatchingInPrefix(*partialPackageFilenamePrefix) == 0) {
            pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);  // Don't mind of duplicates
            hasPackageItsPackageFile = *partiallyMatchedPackageFileElementByPrefix->second == *pkg;

            if (hasPackageItsPackageFile) {
                break;
            }

            ++partiallyMatchedPackageFileElementByPrefix;
        }

        bool isPackageFileForPackageMissing = ! hasPackageItsPackageFile;
        if (isPackageFileForPackageMissing) {
            std::cout << "Missing package file for package: " << *pkg << "\n";
        }
    }

    // MOVING PACKAGE FILES TO SEPARATE DIRECTORY PART

    std::string pathToDuplicateFilesDirectoryAsText = pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";
    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    for (const auto& [packageFile, package] : packageFilesWithPackages) {
        if (package->isEmpty()) {
            const std::string& from = packageFile->getAbsolutePath();
            const std::string& to = pathToDuplicateFilesDirectoryAsText + packageFile->getFilename();
            std::cout << "Moving package file\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
        }
    }

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    return 0;
}
