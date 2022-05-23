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
//    friend bool operator<(const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) {
//        return package->nameAsText < anotherPackage->nameAsText;
//    }

    // Works with comparison with 'name' (unique_ptr<PackageName>)
    //  - with defined friend 'operator<' for with unique_ptr params in 'PackageName' class for direct comparison
    //  - standalone with 'string()' function returning 'std::string'
    friend bool operator<(const std::unique_ptr<Package>& package, const std::unique_ptr<Package>& anotherPackage) {
//        return package->name < anotherPackage->name;
        return package->name->string() < anotherPackage->name->string();
    }

    // Relates to the specialized 'std::less' for this class (at the bottom?)
//    bool operator<(const Package& anotherPackage) const {
//        // Works standalone
//        return Package::nameAsText < anotherPackage.nameAsText;
//
//        // Works with overloaded 'operator<' with 'unique_ptr<PackageName>' type for both parameters in 'PackageName' class
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

// WORKS standalone
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

// WORKS
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
//            // WORKS with overloaded 'operator<' in class 'PackageName' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)'
////            return package->getNameAsInstance() < anotherPackage->getNameAsInstance();
//
//            // WORKS standalone
//            return package->getNameAsInstance()->string() < anotherPackage->getNameAsInstance()->string();
//
//            // WORKS with overloaded 'operator<' in class 'PackageName' defined as 'friend bool operator<(const PackageName& packageName, const PackageName& otherPackageName)'
////            return *package->getNameAsInstance() < *anotherPackage->getNameAsInstance();
//        }
//    };
//}

//template<>
//struct std::less<std::unique_ptr<Package>> {
//    bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
//        // WORKS with overloaded 'operator<' in class 'PackageName' defined as 'friend bool operator<(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)'
////        return package->getNameAsInstance() < anotherPackage->getNameAsInstance();
//
//        // WORKS standalone
//        return package->getNameAsInstance()->string() < anotherPackage->getNameAsInstance()->string();
//
//        // WORKS with overloaded 'operator<' in class 'PackageName' defined as 'friend bool operator<(const PackageName& packageName, const PackageName& otherPackageName)'
////        return *package->getNameAsInstance() < *anotherPackage->getNameAsInstance();
//    }
//};

//template<>
//struct std::greater<std::unique_ptr<Package>> {
//    bool operator() (const unique_ptr<Package>& package, const unique_ptr<Package>& anotherPackage) const {
//        // WORKS with overloaded 'operator>' in class 'PackageName' defined as 'friend bool operator>(const std::unique_ptr<PackageName>& packageName, const std::unique_ptr<PackageName>& otherPackageName)'
////        return package->getNameAsInstance() > anotherPackage->getNameAsInstance();
//
//        // WORKS standalone
////        return package->getNameAsInstance()->string() > anotherPackage->getNameAsInstance()->string();
//
//        // WORKS with overloaded 'operator>' in class 'PackageName' defined as 'friend bool operator>(const PackageName& packageName, const PackageName& otherPackageName)'
//        return *package->getNameAsInstance() > *anotherPackage->getNameAsInstance();
//    }
//};
