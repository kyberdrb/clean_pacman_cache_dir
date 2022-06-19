//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package.h"

#include "ExtendedInstallationPackageFile.h"

#include <iostream>
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
    bool addPackageFileToDeletionCandidates(std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(
            const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles);

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

    bool lessThanOperator(const std::reference_wrapper<Package>& anotherPackage) override {
        return Package::getName() < anotherPackage.get().getName();
    }

private:
    std::unique_ptr<PackageVersion> locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    // TODO make this a 'std::set' to prevent duplicates,
    //  otherwise an exception might occure when deleting an already deleted installation package file, i. e. a duplicate one
    std::vector<std::unique_ptr<ExtendedInstallationPackageFile>> installationPackageFilesForDifferentPackageVersions;
};
