#include "Architectures.h"
#include "Extensions.h"
#include "Package.h"
#include "PackageName.h"

#include "alpm.h"
#include "alpm_list.h"

#include <filesystem>
#include <iostream>
#include <map>

#include <regex>

#include <cassert>

int main() {
    // ALPM PART

    auto* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);

    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);
    alpm_pkg_t* pkg = NULL;
    std::map<std::string, std::string> packages;
    std::set<std::string> packagesToKeep;
    auto architectures = std::make_unique<Architectures>();

    std::map<std::unique_ptr<PackageName>, std::unique_ptr<Package>> locallyInstalledPackages;
    //std::map<std::string, Package> locallyInstalledPackages;

    while (listOfAllLocallyInstalledPackages != NULL) {
        pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        packages.emplace(alpm_pkg_get_name(pkg), alpm_pkg_get_version(pkg));
        architectures->addArchitecture(alpm_pkg_get_arch(pkg));
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageName = alpm_pkg_get_name(pkg);
        auto pkgName = std::make_unique<PackageName>(packageName);

        std::string locallyInstalledVersion = alpm_pkg_get_version(pkg);
        std::string architecture = alpm_pkg_get_arch(pkg);
        auto pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture);

        locallyInstalledPackages.emplace(std::move(pkgName), std::move(pkg));
        //locallyInstalledPackages.emplace(packageName, std::move(pkg));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;

    std::cout << *architectures << "\n";

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF LOCALLY INSTALLED PACKAGES\n\n";

    std::cout << "Found " << packages.size() << " installed packages\n\n";

    for (const auto& [installedPackageName, installedPackageVersion] : packages) {
        std::cout << installedPackageName << "-" << installedPackageVersion << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF LOCALLY INSTALLED PACKAGES IN MAP\n\n";

    std::cout << "Found " << locallyInstalledPackages.size() << " installed packages\n\n";
    assert(packages.size() == locallyInstalledPackages.size());

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

            //std::cout << "Package name:\t\t" << packageName << "\n";
            //std::cout << "Package version:\t" << packageVersion << "\n";
            
            // TODO verify that the extracted package name and extracted package version matches the values already set for the Package instances in the locallyInstalledPackages map
            auto pkgName = std::make_unique<PackageName>(packageName);

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
                std::cout << "Package: '" << packageName << "' has different package file and locally installed version: "
                    << "'" << packageVersion << "' and '" << locallyInstalledVersion << "'" << "\n";
                packageFilesDesignatedForDeletion.emplace(packageRelatedFilename);
            }
        }
    }

    std::cout << *extensions << "\n";

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGES MARKED FOR DELETION\n\n";

    std::cout << "Found " << packageFilesDesignatedForDeletion.size() << " packages designated for deletion\n\n";

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

    return 0;
}
