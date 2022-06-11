//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package_refactored_.h"

#include "ExtendedInstallationPackageFile.h"

#include <iostream>
#include <vector>

class LocallyInstalledPackage_refactored_ : public Package_refactored_ {
public:
    LocallyInstalledPackage_refactored_(
        std::unique_ptr<PackageName> packageName,
        std::unique_ptr<PackageVersion> locallyInstalledVersion,
        std::string architecture,
        bool isIgnored);

    // TODO remove after delegation to the base class 'Package_refactored_'
//    const PackageName& getName() const override;

    uint_fast16_t getNumberOfInstallationPackageFilesForDifferentVersions() const;

    bool addPackageFileToDeletionCandidates(std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(
            const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles);

protected:
    std::ostream& streamOutputOperator(std::ostream& out) const override {
        out

            // TODO remove after delegation to the base class 'Package_refactored_'
//            << *(this->name) << "\t"

            << Package_refactored_::getName() << "\t"
            << *(this->locallyInstalledVersion) << "\t"
            << this->architecture << "\t"
            << "isPackageIgnored: " << this->isIgnored << "\t"

            // TODO remove after delegation to the base class 'Package_refactored_'
//            << *(this->name) << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture;

            << Package_refactored_::getName() << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture;

        if ( ! (this->installationPackageFilesForDifferentPackageVersions.empty() ) ) {
            for (const auto& packageRelatedFile: this->installationPackageFilesForDifferentPackageVersions) {
                out << "\n";
                out << "  - " << *packageRelatedFile;
            }
        }

        return out;
    }

    bool lessThanOperator(const std::unique_ptr<Package_refactored_>& anotherPackage) const override {
        return Package_refactored_::getName() < anotherPackage->getName();
    }

    bool lessThanOperator(const std::reference_wrapper<Package_refactored_>& anotherPackage) override {
        return Package_refactored_::getName() < anotherPackage.get().getName();
    }

private:
    // TODO remove after delegation to the base class 'Package_refactored_'
//    std::unique_ptr<PackageName> name;

    std::unique_ptr<PackageVersion> locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<ExtendedInstallationPackageFile>> installationPackageFilesForDifferentPackageVersions;
};
