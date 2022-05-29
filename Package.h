#pragma once

#include "ExtendedInstallationPackageFile.h"
#include "PackageName.h"
#include "PackageVersion.h"

#include <vector>

class Package {
public:
    Package(std::unique_ptr<PackageName> packageName, std::unique_ptr<PackageVersion> locallyInstalledVersion, std::string architecture, bool isIgnored);

    explicit Package(std::unique_ptr<PackageName> inferredPackageName);

    const PackageName& getName() const;

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    uint_fast8_t getStartingPositionForPackageVersion() const;

    uint_fast16_t getNumberOfInstallationPackageFilesForDifferentVersions() const;

    void addPackageFileToDeletionCandidates(std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(
            const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles);

    friend std::ostream& operator<<(std::ostream& out, const Package& package) {
        out << *(package.name) << "\t";

        // Print the rest of fully constructed package
        if ( ! package.locallyInstalledVersion->empty() ) {
            out
                    << *(package.locallyInstalledVersion) << "\t"
                    << package.architecture << "\t"
                    << "isPackageIgnored: " << package.isIgnored << "\t"
                    << *(package.name) << "-" << *(package.locallyInstalledVersion) << "-" << package.architecture;

            if ( ! (package.installationPackageFilesForDifferentPackageVersions.empty() ) ) {
                for (const auto& packageRelatedFile: package.installationPackageFilesForDifferentPackageVersions) {
                    out << "\n";
                    out << "  - " << *packageRelatedFile;
                }
            }
        }

        return out;
    }

    friend bool operator<(const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) {
        // Delegate comparison to overloaded 'operator<' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)' in 'PackageName' class
        return package->name < anotherPackage->name;
    }

private:
    std::unique_ptr<PackageName> name;
    std::unique_ptr<PackageVersion> locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<ExtendedInstallationPackageFile>> installationPackageFilesForDifferentPackageVersions;
};
