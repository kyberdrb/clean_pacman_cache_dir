#pragma once

#include "PackageFile.h"

#include <string>
#include <vector>
#include <ostream>

class Package {
public:
    Package(std::string name, std::string locallyInstalledVersion, std::string architecture, bool isIgnored);

    explicit Package(std::string inferredPackageName);

    std::string getName() const;

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    uint8_t getStartingPositionForPackageVersion() const;

    bool hasInstallationPackageFilesForDifferentVersions() const;

    void addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles);

    friend std::ostream& operator<<(std::ostream& out, const Package& package) {
        out << package.name << "\t";

        // Print the rest of fully constructed package
        if ( ! package.locallyInstalledVersion.empty() ) {
            out
                    << package.locallyInstalledVersion << "\t"
                    << package.architecture << "\t"
                    << "isPackageIgnored: " << package.isIgnored << "\t"
                    << package.name << "-" << package.locallyInstalledVersion << "-" << package.architecture;

            if (!package.name.empty() && std::isdigit(package.name.at(0))) {
                out << "\t" << "PACKAGE NAME BEGINNS WITH A NUMBER";
            }

            if (!package.locallyInstalledVersion.empty() && !std::isdigit(package.locallyInstalledVersion.at(0))) {
                out << "\t" << "PACKAGE VERSION BEGINNS WITH A LETTER";
            }

            if (!package.packageFilesForDeletion.empty()) {
                for (const auto& packageRelatedFile: package.packageFilesForDeletion) {
                    out << "\n";
                    out << "  - " << *packageRelatedFile;
                }
            }
        }

        return out;
    }

    // Relates to the specialized 'std::less' for this class (at the bottom?)
    bool operator<(const Package& anotherPackage) const {
        return Package::name < anotherPackage.name;
    }

private:
    std::string name;
    std::string locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<PackageFile>> packageFilesForDeletion;
};

// specialize 'std::less' functor to enable lookup with 'set::find' in a 'std::set' or a 'map'
//  with instances of this class as an element (std::set) / key (std::map), or with any custom object-type key
namespace std {
    template<>
    struct less<unique_ptr<Package>> {
        bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
            return *package < *anotherPackage;
        }
    };
}
