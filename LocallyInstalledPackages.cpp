//
// Created by laptop on 6/17/22.
//

#include "LocallyInstalledPackages.h"

#include "PackageVersion.h"
#include "LocallyInstalledPackage.h"
#include "PackageNameMissing.h"

#include "alpm.h"
#include "alpm_list.h"

#include <filesystem>

#include <cassert>

LocallyInstalledPackages::LocallyInstalledPackages(const IgnoredPackageNames& ignoredPackageNames) :
        ignoredPackageNames(ignoredPackageNames)
{
    this->fillLocallyInstalledPackages();
}

void LocallyInstalledPackages::fillLocallyInstalledPackages() {
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

        auto ignoredPackageNameCandidate = std::make_unique<IgnoredPackageName>(std::move(packageNameAsText));

        // For debugging purposes - if the argument is passed by value to a function, which accepts the argument as a value
        //  the argument is __copied__ from the calling to the receiving function
//        assert(packageNameAsText != "");

        bool isIgnored = ignoredPackageNames.isPackageWithGivenNameIgnored(ignoredPackageNameCandidate);
        auto packageName = std::make_unique<PackageName>(ignoredPackageNameCandidate->moveNameFromThisInstance());

        // For debugging purposes - if the argument is passed by value with 'std::move' to a function,
        //  which accepts the argument as a value, the argument is __moved__ from the instance member variable
        //  to the variable in the caller function
//        std::stringstream ignoredPackageNameCandidateAsText;
//        ignoredPackageNameCandidateAsText << *ignoredPackageNameCandidate;
//        assert(ignoredPackageNameCandidateAsText.str() == "");

        auto locallyInstalledVersion = std::make_unique<PackageVersion>(std::move(locallyInstalledVersionAsText));

        auto locallyInstalledPackage = std::make_unique<LocallyInstalledPackage>(
                std::move(packageName),
                std::move(locallyInstalledVersion),
                architecture,
                isIgnored);

        addLocallyInstalledPackage(std::move(locallyInstalledPackage));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;
}

void LocallyInstalledPackages::addLocallyInstalledPackage(
        std::unique_ptr<LocallyInstalledPackage> locallyInstalledPackage)
{
    this->locallyInstalledPackages.emplace(std::move(locallyInstalledPackage));
}

const Package& LocallyInstalledPackages::find(std::unique_ptr<Package>& packageWithInferredName) const {
    auto iteratorPointingToMatchingLocallyInstalledPackage = this->locallyInstalledPackages.find(packageWithInferredName);

    bool packageWithInferredNameIsMissing = iteratorPointingToMatchingLocallyInstalledPackage == this->locallyInstalledPackages.end();
    if (packageWithInferredNameIsMissing) {
        // throwing an exception 'PackageNameMissing' and catching it in main
        //  instead of using a 'Null Object' design pattern and override virtual methods
        //  that are irrelevant to implement for the 'Null Object' - violation of LSP - Liskov Substitution Principle
        throw PackageNameMissing(*packageWithInferredName);
    }

    const std::unique_ptr<Package>& uniquePtrBehindIteratorForMatchingLocallyInstalledPackage = *(iteratorPointingToMatchingLocallyInstalledPackage);
    const Package& instanceBehindUniquePtrForMatchingLocallyInstalledPackage = *(uniquePtrBehindIteratorForMatchingLocallyInstalledPackage);
    return instanceBehindUniquePtrForMatchingLocallyInstalledPackage;
}

std::string LocallyInstalledPackages::generateReport() const {
    std::stringstream report;
    
    report << "\n";
    report << "===============================================\n\n";
    report << "LIST OF ALL INSTALLED PACKAGES WITH RELATED PACKAGE FILES FOR DIFFERENT VERSIONS (IF ANY)\n\n";

    report << "Found " << this->locallyInstalledPackages.size() << " installed packages\n\n";

    for (const auto& package : this->locallyInstalledPackages) {
        report << *package << "\n";
    }

    report << "\n";
    report << "===============================================\n\n";
    report << "LIST OF INSTALLED PACKAGES THAT HAVE AT LEAST ONE RELATED INSTALLATION PACKAGE FILE FOR DIFFERENT VERSION THAN THE LOCALLY INSTALLED ONE\n\n";

    report << "Found " << packagesWithInstallationPackageFilesForDifferentVersions.size()
           << " installed packages with installation package files for other than locally installed version\n\n";

    uint_fast16_t numberOfInstallationPackageFilesForOtherVersions = 0;

    for (const auto package : packagesWithInstallationPackageFilesForDifferentVersions) {
        report << package << "\n";

        uint_fast16_t numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage =
                package.get().getNumberOfInstallationPackageFilesForDifferentVersions();

        numberOfInstallationPackageFilesForOtherVersions += numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage;
    }

    report << "\n";
    report << "Found " << numberOfInstallationPackageFilesForOtherVersions
           << " installation package files for different version than the locally installed\n";

    return report.str();
}

void LocallyInstalledPackages::movePackageFilesForDifferentPackageVersionsToSeparateDir() const {
    std::string pathToDuplicateFilesDirectoryAsText =
            this->pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";

    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    auto directoryForInstallationPackageFilesForDeletion = std::make_unique<AbsolutePath>(pathToDuplicateFilesDirectoryAsText);

    for (const auto& installedPackage : packagesWithInstallationPackageFilesForDifferentVersions) {
        installedPackage.get().movePackageFilesForDifferentVersionsToSeparateDir( *(directoryForInstallationPackageFilesForDeletion) );
    }
}

void LocallyInstalledPackages::addReferenceToPackageRelatedToInstallationPackageFileForDifferentVersion(const LocallyInstalledPackage& localyInstalledPackageExactModifiable) {
    this->packagesWithInstallationPackageFilesForDifferentVersions.emplace(localyInstalledPackageExactModifiable);
}
