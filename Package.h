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

    void getNextInferredPackageNameCandidate();

    bool hasStillSomethingInPackageName() const;

    bool isPackageNameEmpty() const;

    uint8_t getStartingPositionForPackageVersion() const;

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

// FOR DEREFERENCED (SMART) POINTER COMPARISON

    // Works for dereferenced comparison together with overloaded 'std::less' funcion for cutom type
    bool operator<(const Package& package) const {
        // TODO maybe replace the 'getName()' function with only fields?
        return this->getName() < package.getName();
//        return this->name < package.getName();
//        return Package::name < package.getName();
//        return Package::name < package.name;
    }


// FOR DIRECT (SMART) POINTER COMPARISON

    // Doesn't work
//    bool operator<(const std::unique_ptr<Package>& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//        return this->getName() < package->getName();
////        return this->name < package.getName();
////        return Package::name < package.getName();
////        return Package::name < package.name;
//    }

//    // WORKS for direct comparison without overloading 'std::less' funcion
//    friend bool operator<(const std::unique_ptr<Package>& lhs, const std::unique_ptr<Package>& rhs) {
//        return lhs->name < rhs->name;
//    }

    // Doesn't work
//    friend bool operator<(std::unique_ptr<Package>& lhs, std::unique_ptr<Package>& rhs) {
//        return lhs->name < rhs->name;
//    }

private:
    std::string name;
    std::string locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<PackageFile>> packageFilesForDeletion;
};

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
namespace std {
    template<>
    struct less<unique_ptr<Package>> {
        bool operator() (const unique_ptr<Package>& lhs, const unique_ptr<Package>& rhs) const {
            return *lhs < *rhs;
        }
    };
}

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& lhs, const unique_ptr<Package>& rhs) const {
//            return lhs->getName() < rhs->getName();
//        }
//    };
//}
