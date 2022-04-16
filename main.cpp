#include "Architectures.h"
#include "Extensions.h"
#include "Package.h"
#include "PackageName.h"

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
    std::regex regexForIgnoredPackagesInPacmanConfigFile("IgnorePkg = ");

    while (std::getline(pacmanConfigFile, lineWithIgnoredPackages)) {
        // does the lineWithIgnoredPackages contain 'IgnorePkg' text?
        bool doesTheLineContainIgnoredPackages = std::regex_search(lineWithIgnoredPackages, match, regexForIgnoredPackagesInPacmanConfigFile);
        if (doesTheLineContainIgnoredPackages) {
            break;
        }
    }

    // TODO OPTIONAL (assuming no leading spaces/tabs) remove leading and ending blank characters
    // TODO OPTIONAL (assuming no ending spaces/tabs; only one space delimiting [separating] each package name) replace multiple spaces or tabs with one space
    // tokenize the line by space in order to build a list of ignored packages

    std::stringstream ignoredPackagesAsStream{lineWithIgnoredPackages};
    std::string ignoredPackageNameAsText{};
    std::vector<std::string> ignoredPackageNamesInTextFormat{};

    char delimiterForIgnoredPakcages = ' ';
    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPakcages)) {
        ignoredPackageNamesInTextFormat.push_back(ignoredPackageNameAsText);
    }

//    ignoredPackageNameAsText.clear();
//
//    // iterate through all ignoredPackageNames
//    for (auto& ignoredPackageNameAsText : ignoredPackageNamesInTextFormat) {
//        //  check whether the packageName exists in the locallyInstalledPackages
//        //  if yes, then
//        //    move the Package (the value) associated with the ignoredPackageName (the key) from the locallyInstalledPackages to the ignoredPakcages
//        //    delete the entry from the locallyInstalledPackages associated with the packageName (in order to have only one copy of each package
//        //                                                                                        and in order to have the packages divided into ignored and active)
//
//        auto pkgName = std::make_unique<PackageName>(std::move(ignoredPackageNameAsText));
//        bool isPackageNameLocallyInstalled = locallyInstalledPackages.count(std::move(pkgName));
//        if (isPackageNameLocallyInstalled) {
//            pkgName = std::make_unique<PackageName>(std::move(ignoredPackageNameAsText));
//            auto pkgMovedFromLocallyInstalledToIgnoredPackages = locallyInstalledPackages.at(std::move(pkgName)).get();
//            ignoredPackages.emplace(pkgName, std::move(pkgMovedFromLocallyInstalledToIgnoredPackages));
//        }
//    }

    // ALPM PART

    alpm_errno_t* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);

    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);
    alpm_pkg_t* pkg = NULL;
    std::map<std::string, std::string> allPackagesInTextFormat;
    std::set<std::string> packagesToKeep;
    auto architectures = std::make_unique<Architectures>();

    std::map<std::unique_ptr<PackageName>, std::unique_ptr<Package>> locallyInstalledPackages;

    while (listOfAllLocallyInstalledPackages != NULL) {
        pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        allPackagesInTextFormat.emplace(alpm_pkg_get_name(pkg), alpm_pkg_get_version(pkg));
        architectures->addArchitecture(alpm_pkg_get_arch(pkg));
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageName = alpm_pkg_get_name(pkg);
        auto pkgName = std::make_unique<PackageName>(packageName);

        std::string locallyInstalledVersion = alpm_pkg_get_version(pkg);
        std::string architecture = alpm_pkg_get_arch(pkg);
        auto pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);

        if(std::find(ignoredPackageNamesInTextFormat.begin(), ignoredPackageNamesInTextFormat.end(), packageName) != ignoredPackageNamesInTextFormat.end()) {
            ignoredPackages.emplace(std::move(pkgName), std::move(pkg));
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
    std::cout << "LIST OF LOCALLY INSTALLED PACKAGES\n\n";

    std::cout << "Found " << allPackagesInTextFormat.size() << " installed packages\n\n";

    for (const auto& [installedPackageName, installedPackageVersion] : allPackagesInTextFormat) {
        std::cout << installedPackageName << "\t" << installedPackageVersion << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF LOCALLY INSTALLED PACKAGES IN MAP WITH CUSTOM-OBJECT KEYS\n\n";

    std::cout << "Found " << locallyInstalledPackages.size() << " installed packages\n";
    if (locallyInstalledPackages.size() <= allPackagesInTextFormat.size()) {
        std::cout << "Found " << ignoredPackages.size() << " ignored packages\n";
        std::cout << "Found " << locallyInstalledPackages.size() + ignoredPackages.size() << " packages altogether\n\n";
        assert(locallyInstalledPackages.size() + ignoredPackages.size() == allPackagesInTextFormat.size());
    }

    for (const auto& [installedPackageName, package] : locallyInstalledPackages) {
        std::cout << *installedPackageName << "\t" << *package << "\n";
    }

    // FILESYSTEM PART

    std::filesystem::path aPath {"/var/cache/pacman/pkg"};
    std::set<std::string> downloadedPackages;
    auto extensions = std::make_unique<Extensions>();
    std::set<std::string> packageFilesDesignatedForDeletion;

    for (const auto& packageRelatedFile : std::filesystem::directory_iterator(aPath)) {
        const auto& packageRelatedFilename = packageRelatedFile.path().filename().string();
        const auto& extension = packageRelatedFile.path().extension().string();
        if (packageRelatedFile.is_regular_file() ) {
            // for debugging purposes
//            std::cout << packageRelatedFilename << "\n";
            if (extension == ".part") {
                packageFilesDesignatedForDeletion.emplace(packageRelatedFilename);
                continue;
            }

            downloadedPackages.emplace(packageRelatedFilename);
            extensions->addExtension(extension);

            // REGEX REPLACE TESTING IN THE LOOP THAT ITERATES THE PACKAGE FILES

            std::string pkgFilename = packageRelatedFilename;
            //std::cout << "Package name:\n";
            //std::cout << pkgFilename << "\n";

            // TODO use 'Architectures' class to access all available architectures to build the regex pattern
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
            //  name     version

            for (const auto& token : tokens) {
                // TODO handle (rare) cases for package names with numbers as the first character e.g. '0ad' and  after the dash e.g. '-1.16'
                //  and package version that begin with a letter e.g. '	a25.b-3'
                //   - adjust 'operator<'?
                //   - write a custom comparator that will check character by character for prefix similarity?
                //   - add them to ignoredPackages?
                //   - add them to unhandled/other packages and then handle manually?/automatically?
                //   - assemble a package filename by trial-and-error from available extensions and check whether the file under assembles filename exists?
                // Problematic package filenames use-cases:
                //  libyuv-r2212+dfaf7534-2-x86_64.pkg.tar.zst
                //  libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst
                //  libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst.sig
                // this parsing algorithm breaks when the first character of any of the tokens belonging to the package name is a number
                //  - then packageName stays empty or incomplete, the packageVersion has the rest of the package name together with the version
                //  or when the package version has as the first token a letter
                bool isFirstCharacterOfTokenCharacter = !isdigit(token.at(0));

                if (isFirstCharacterOfTokenCharacter && hasPackageNameMoreTokens) {
                    packageName += token + delimiter;
                    continue;
                }

                hasPackageNameMoreTokens = false;

                packageVersion += token + delimiter;
            }

            packageName.pop_back();
            packageVersion.pop_back();

            //std::cout << "Package name:\t\t" << packageName << "\n";
            //std::cout << "Package version:\t" << packageVersion << "\n";

            // for debugging purposes - pakcage with longer version
//            if (packageName == "adobe-source-code-pro-fonts") {
//                auto p = static_cast<std::string*>(&packageName);
//            }

            auto pkgName = std::make_unique<PackageName>(packageName);
            if (ignoredPackages.count(pkgName) == 1) {
                std::cout << "Package '" << packageName << "' is marked as ignored and listed in pacman's configuration file in 'IgnorePkg' entry" << "\n";
                continue;
            }

            // TODO verify that the extracted package name and extracted package version matches the values already set for the Package instances in the locallyInstalledPackages map
            pkgName = std::make_unique<PackageName>(packageName);

            if (locallyInstalledPackages.count(std::move(pkgName)) == 0) {
                std::cout << "Package: '" << packageName << "' not found. The package is no longer present in the system, "
                                                            "or the package had been repackaged to another name, "
                                                            "or the package had been uninstalled" << "\n";
                packageFilesDesignatedForDeletion.emplace(packageRelatedFilename);
                continue;
            }

            pkgName = std::make_unique<PackageName>(packageName);
            const std::string&& locallyInstalledVersion = locallyInstalledPackages.at(std::move(pkgName))->getLocallyInstalledVersion();
            if (packageVersion != locallyInstalledVersion) {    // possibly nullptr (key not found) and consequent crash on error
                // for debugging purposes
                //std::cout << "Package: '" << packageName << "' has different package file and locally installed version: " << "'" << packageVersion << "' and '" << locallyInstalledVersion << "'" << "\n";

                packageFilesDesignatedForDeletion.emplace(packageRelatedFilename);
            }
        }
    }

    std::cout << *extensions << "\n";

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
    std::cout << "Package name:\n";
    std::cout << pkgFilename << "\n";

    // TODO use 'Architectures' class to access all available architectures to build the regex pattern
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

    std::cout << "Package name:\t\t" << packageName << "\n";
    std::cout << "Package version:\t" << packageVersion << "\n";

    std::cout << "\n";

    // TODO add report which packages have 0 packageVersionsWithRelatedDownloadedFiles

    return 0;
}
