#pragma once

#include "PackageFile.h"

#include <string>
#include <vector>
#include <ostream>

class Package {
public:
    Package(std::string name, std::string locallyInstalledVersion, std::string architecture);
    Package(std::string name, std::string locallyInstalledVersion, std::string architecture, bool isIgnored);

    explicit Package(std::string inferredPackageName);

    std::string getName() const;

    std::string getLocallyInstalledVersion() const;

    bool addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles);





    bool isSpecial() const;

    std::string buildPartialPackageNamePrefix() const;

    bool isEmpty() const;

    // TODO maybe delete this function altogether
    std::vector<std::string> getPackageNameCandidates() const;

    friend std::ostream& operator<<(std::ostream& out, const Package& package) {
        out << package.name << "\t" << package.locallyInstalledVersion << "\t" << package.architecture << "\t" << "isPackageIgnored: " << package.isIgnored << "\t" << package.name << "-" << package.locallyInstalledVersion << "-" << package.architecture;

        if ( ! package.name.empty() && std::isdigit(package.name.at(0) ) ) {
            out << "\t" << "PACKAGE NAME BEGINNS WITH A NUMBER";
        }

        if ( ! package.locallyInstalledVersion.empty() && ! std::isdigit(package.locallyInstalledVersion.at(0) ) ) {
            out << "\t" << "PACKAGE VERSION BEGINNS WITH A LETTER";
        }

        if ( ! package.packageFilesForDeletion.empty() ) {
            for (const auto& packageRelatedFile: package.packageFilesForDeletion) {
                out << "\n";
                out << "  - " << *packageRelatedFile;
            }
        }

        return out;
    }

    bool operator==(const Package& package) const {
        return name == package.name &&
               locallyInstalledVersion == package.locallyInstalledVersion &&
               architecture == package.architecture;
    }

    bool operator<(const Package& package) const {
        return this->getName() < package.getName();
    }

//    bool operator==(const PackageFile& packageFile) const {
//        return name == packageFile.getExtractedPackageName();
//    }

private:
    std::string name;
    std::string locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<PackageFile>> packageFilesForDeletion;
};

namespace std {
    template<>
    struct less<unique_ptr<Package>> {
        bool operator() (const unique_ptr<Package>& lhs, const unique_ptr<Package>& rhs) const {
            return *lhs < *rhs;
        }
    };
}
