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
    void commonFunction() override {
        std::cout << "Common function - from LocallyInstalledPackage__refactored__" << "\n";
    }

    void functionOnlyInLocallyInstalledPackage_refactored_() {
        std::cout << "Function only in LocallyInstalledPackage__refactored__" << "\n";
    }

    // For testing purposes
    LocallyInstalledPackage_refactored_() = default;

    LocallyInstalledPackage_refactored_(std::unique_ptr<PackageName> packageName, std::unique_ptr<PackageVersion> locallyInstalledVersion, std::string architecture, bool isIgnored);

    const PackageName& getName() const override;

    uint_fast16_t getNumberOfInstallationPackageFilesForDifferentVersions() const;

    bool addPackageFileToDeletionCandidates(std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(
            const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles);

//    friend std::ostream& operator<<(std::ostream& out, const LocallyInstalledPackage_refactored_& package) {
//        out << *(package.name) << "\t";
//
//        // Print the rest of fully constructed package
//        if ( ! package.locallyInstalledVersion->empty() ) {
//            out
//                    << *(package.locallyInstalledVersion) << "\t"
//                    << package.architecture << "\t"
//                    << "isPackageIgnored: " << package.isIgnored << "\t"
//                    << *(package.name) << "-" << *(package.locallyInstalledVersion) << "-" << package.architecture;
//
//            if ( ! (package.installationPackageFilesForDifferentPackageVersions.empty() ) ) {
//                for (const auto& packageRelatedFile: package.installationPackageFilesForDifferentPackageVersions) {
//                    out << "\n";
//                    out << "  - " << *packageRelatedFile;
//                }
//            }
//        }
//
//        return out;
//    }

//    friend bool operator<(const std::unique_ptr<LocallyInstalledPackage_refactored_>& package, const std::unique_ptr<LocallyInstalledPackage_refactored_>& anotherPackage) {
//        // Delegate comparison to overloaded 'operator<' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)' in 'PackageName' class
//        return package->name < anotherPackage->name;
//    }

//    friend bool operator<(
//            const std::reference_wrapper<LocallyInstalledPackage_refactored_>& package,
//            const std::reference_wrapper<LocallyInstalledPackage_refactored_>& anotherPackage)
//    {
//        return package.get().name < anotherPackage.get().name;
//    }

protected:
    std::ostream& streamOutputOperator(std::ostream& out) const override {
        out << *(this->name) << "\t";

        // Print the rest of fully constructed package
        if ( ! this->locallyInstalledVersion->empty() ) {
            out
                    << *(this->locallyInstalledVersion) << "\t"
                    << this->architecture << "\t"
                    << "isthis->gnored: " << this->isIgnored << "\t"
                    << *(this->name) << "-" << *(this->locallyInstalledVersion) << "-" << this->architecture;

            if ( ! (this->installationPackageFilesForDifferentPackageVersions.empty() ) ) {
                for (const auto& packageRelatedFile: this->installationPackageFilesForDifferentPackageVersions) {
                    out << "\n";
                    out << "  - " << *packageRelatedFile;
                }
            }
        }

        return out;
    }

    bool lessThanOperator(const std::unique_ptr<Package_refactored_>& anotherPackage) const override {
        return *(this->name) < anotherPackage->getName();
    }

private:
    std::unique_ptr<PackageName> name;
    std::unique_ptr<PackageVersion> locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<ExtendedInstallationPackageFile>> installationPackageFilesForDifferentPackageVersions;
};
