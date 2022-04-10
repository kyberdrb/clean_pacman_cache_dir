#include "Architectures.h"
#include "Extensions.h"
#include "Package.h"

#include "alpm.h"
#include "alpm_list.h"

#include <filesystem>
#include <iostream>
#include <map>

int main() {
    std::filesystem::path aPath {"/var/cache/pacman/pkg"};
    std::set<std::string> downloadedPackages;
    auto extensions = std::make_unique<Extensions>();
    std::set<std::string> packageFilesDesignatedForDeletion;

    for (const auto& entry : std::filesystem::directory_iterator(aPath)) {
        const auto& filename = entry.path().filename().string();
        const auto& extension = entry.path().extension().string();
        if (entry.is_regular_file() ) {
            // for debugging purposes
//            std::cout << filename << "\n";
            if (extension == ".part") {
                packageFilesDesignatedForDeletion.emplace(filename);
                continue;
            }

            downloadedPackages.emplace(filename);
            extensions->addExtension(extension);
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

    auto* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);

    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);
    alpm_pkg_t* pkg = NULL;
    std::map<std::string, std::string> packages;
    std::set<std::string> packagesToKeep;
    auto architectures = std::make_unique<Architectures>();

    while (listOfAllLocallyInstalledPackages != NULL) {
        pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        packages.emplace(alpm_pkg_get_name(pkg), alpm_pkg_get_version(pkg));
        architectures->addArchitecture(alpm_pkg_get_arch(pkg));
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);
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

    std::cout << "\n";

    return 0;
}
