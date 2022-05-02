//
// Created by laptop on 5/2/22.
//

#include "LocallyInstalledPackages.h"

#include "alpm.h"

#include <sstream>

LocallyInstalledPackages::LocallyInstalledPackages(const std::unique_ptr<IgnoredPackageNames>& ignoredPackageNames) :
        ignoredPackageNames(ignoredPackageNames)
{
    this->findLocallyInstalledPackages();
}

void LocallyInstalledPackages::findLocallyInstalledPackages() {
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
//            out << "Here we go..." << "\n";
//        }

        auto packageName = std::make_unique<PackageName>(packageNameAsText);
        bool isIgnored = this->ignoredPackageNames->contains(*packageName);
        auto package = std::make_unique<Package>(packageNameAsText, locallyInstalledVersion, architecture, isIgnored);
        this->installedPackages.emplace(std::move(package));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;
}

std::ostream& operator<<(std::ostream& out, const LocallyInstalledPackages& locallyInstalledPackages) {
    out << "\n";
    out << "===============================================\n\n";
    out << "LIST OF INSTALLED PACKAGES WITH/WITHOUT RELATED PACKAGE FILES FOR DIFFERENT VERSIONS\n\n";

    out << "Found " << locallyInstalledPackages.installedPackages.size() << " installed packages\n\n";

    for (const auto& package : locallyInstalledPackages.installedPackages) {
        out << *package << "\n";
    }

    return out;
}

std::string LocallyInstalledPackages::printInstalledPackages() const {
    std::stringstream textRepresentationsOfInstanceAsStream{};
    textRepresentationsOfInstanceAsStream << *this;
    return textRepresentationsOfInstanceAsStream.str();
}
