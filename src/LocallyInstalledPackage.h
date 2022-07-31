//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package.h"

#include "ExtendedInstallationPackageFile.h"

#include <vector>

class LocallyInstalledPackage : public Package {
public:
    LocallyInstalledPackage(
        std::unique_ptr<PackageName> packageName,
        std::unique_ptr<PackageVersion> locallyInstalledVersion,
        std::string architecture,
        bool isIgnored);

    uint_fast16_t getNumberOfInstallationPackageFilesForDifferentVersions() const;

    /** Adds package file to the deletion candidates
     * @param packageRelatedPackageFile package file to be added in the container of package files for deletion
     * @return TRUE if the package file was added to the container of package files for deletion
     * or false when package names are different, package version is matching or package is ignored
     */
    bool addPackageFileToDeletionCandidatesOnlyWhenMatchingCriteria(std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(
            const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles) const;

    friend bool operator<(
            const std::reference_wrapper<const LocallyInstalledPackage>& locallyInstalledPackage,
            const std::reference_wrapper<const LocallyInstalledPackage>& anotherLocallyInstalledPackage)
    {
        return locallyInstalledPackage.get().getName() < anotherLocallyInstalledPackage.get().getName();
    }

protected:
    std::ostream& streamOutputOperator(std::ostream& out) const override {
        out
                << Package::getName() << "\t"
                << *(this->locallyInstalledVersion) << "\t"
                << this->architecture << "\t"
                << "isPackageIgnored: " << this->isIgnored << "\t"
                << Package::getName() << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture;

        if ( ! (this->installationPackageFilesForDifferentPackageVersions.empty() ) ) {
            for (const auto& packageRelatedFile: this->installationPackageFilesForDifferentPackageVersions) {
                out << "\n";
                out << "  - " << *packageRelatedFile;
            }
        }

        return out;
    }

    bool lessThanOperator(const std::unique_ptr<Package>& anotherPackage) const override {
        return Package::getName() < anotherPackage->getName();
    }

private:
    std::unique_ptr<PackageVersion> locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    // I was considering to change the type of this container from 'std::vector' to 'std::set'
    //  to prevent adding duplicate package file entry,
    //  and to prevent an exception which might occure at deleting an already deleted installation package file, i. e. the duplicate one
    //  but I decided to keep this as 'vector' because that way I can spot where in my code I generate duplicate entries for package files.
    //  But maybe I will eventually change the type of this member variable to 'std::set' when I'll be doing a release
    std::vector<std::unique_ptr<ExtendedInstallationPackageFile>> installationPackageFilesForDifferentPackageVersions;
};
