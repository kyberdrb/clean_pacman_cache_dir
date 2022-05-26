#pragma once

#include "PackageFile.h"
#include "PackageName.h"
#include "PackageVersion.h"

#include <string>
#include <vector>
#include <ostream>

class Package {
public:
    Package(std::unique_ptr<PackageName> packageName, std::unique_ptr<PackageVersion> locallyInstalledVersion, std::string architecture, bool isIgnored);

    explicit Package(std::unique_ptr<PackageName> inferredPackageName);

    const PackageName& getNameAsReference() const;

    const PackageVersion& getRelatedPackageVersionAsReference() const;

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    uint8_t getStartingPositionForPackageVersion() const;

    uint_fast16_t getNumberOfInstallationPackageFilesForDifferentVersions() const;

    void addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles);

    friend std::ostream& operator<<(std::ostream& out, const Package& package) {
        out << *(package.name) << "\t";

        // Print the rest of fully constructed package
        if ( ! package.locallyInstalledVersion->empty() ) {
            out
                    << *(package.locallyInstalledVersion) << "\t"
                    << package.architecture << "\t"
                    << "isPackageIgnored: " << package.isIgnored << "\t"
                    << *(package.name) << "-" << *(package.locallyInstalledVersion) << "-" << package.architecture;

            if ( ! (package.packageFilesForDeletion.empty() ) ) {
                for (const auto& packageRelatedFile: package.packageFilesForDeletion) {
                    out << "\n";
                    out << "  - " << *packageRelatedFile;
                }
            }
        }

        return out;
    }

    friend bool operator<(const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) {
        return package->name < anotherPackage->name; // Delegate comparison to overloaded 'operator<' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)' in 'PackageName' class
    }

private:
    std::unique_ptr<PackageName> name;
    std::unique_ptr<PackageVersion> locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<PackageFile>> packageFilesForDeletion;
};
