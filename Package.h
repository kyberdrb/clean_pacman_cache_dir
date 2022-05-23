#pragma once

#include "PackageFile.h"
#include "PackageName.h"

#include <string>
#include <vector>
#include <ostream>

class Package {
public:
    Package(std::string name, std::unique_ptr<PackageName> packageName, std::string locallyInstalledVersion, std::string architecture, bool isIgnored);

    explicit Package(std::string inferredPackageNameAsText, std::unique_ptr<PackageName> inferredPackageName);

    std::string getName() const;
    std::unique_ptr<PackageName>& getNameAsInstance();

    bool isPackageNameEmpty() const;

    bool hasStillSomethingInPackageName() const;

    void getNextInferredPackageNameCandidate();

    uint8_t getStartingPositionForPackageVersion() const;

    uint_fast16_t getNumberOfInstallationPackageFilesForDifferentVersions() const;

    void addPackageFileToDeletionCandidates(std::unique_ptr<PackageFile> packageRelatedPackageFile);

    void movePackageFilesForDifferentVersionsToSeparateDir(std::string pathToDirectoryForOtherVersionsOfPackageFiles);

    friend std::ostream& operator<<(std::ostream& out, const Package& package) {
        out << package.nameAsText << "\t" << *package.name << "\t";

        // Print the rest of fully constructed package
        if ( ! package.locallyInstalledVersion.empty() ) {
            out
                    << package.locallyInstalledVersion << "\t"
                    << package.architecture << "\t"
                    << "isPackageIgnored: " << package.isIgnored << "\t"
                    << package.nameAsText << "-" << package.locallyInstalledVersion << "-" << package.architecture;

            if (!package.nameAsText.empty() && std::isdigit(package.nameAsText.at(0))) {
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

    // WORKS with comparison with 'nameAsText' (string)
    // Doesn't work with comparing 'name' (unique_ptr<PackageName>) even with defined friend 'operator<' for these types
    // Doesn't work even when comparing unique_ptr<PackageName> by the 'string()' function within the pointer
    // I don't know what is going on... Why comparing internal strings finds an element,
    //  but comparing external strings doesn't...
    friend bool operator<(const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) {
        return package->nameAsText < anotherPackage->nameAsText;
//        return package->name < anotherPackage->name;
//        return package->name->string() < anotherPackage->name->string();
    }

//    friend bool operator<(
//            std::unique_ptr<PackageName>& packageName,
//            std::unique_ptr<PackageName>& otherPackageName)
//    {
//        return packageName->string() < otherPackageName->string();
//    }

    // Relates to the specialized 'std::less' for this class (at the bottom?)
//    bool operator<(const Package& anotherPackage) const {
//        return Package::nameAsText < anotherPackage.nameAsText;
////        return Package::name < anotherPackage.name;
//    }

private:
    std::string nameAsText;
    std::unique_ptr<PackageName> name;

    std::string locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<PackageFile>> packageFilesForDeletion;
};

// WORKS on its own
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
//            return package->getName() < anotherPackage->getName();
//        }
//    };
//}

// WORKS with member 'operator<' defined as 'bool operator<(const Package& anotherPackage) const'
// specialize 'std::less' functor to enable lookup with 'set::find' in a 'std::set' or a 'map'
//  with instances of this class as an element (std::set) / key (std::map), or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
//            return *package < *anotherPackage;
//        }
//    };
//}

// Doesn't work. Why? It's comparing strings, just like in previous example, or not?
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
////            return package->getNameAsInstance() < anotherPackage->getNameAsInstance();
//            return package->getNameAsInstance()->string() < anotherPackage->getNameAsInstance()->string();
//        }
//    };
//}
