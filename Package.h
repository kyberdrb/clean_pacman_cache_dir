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

// FOR DIRECT (SMART) POINTER COMPARISON FOR 'SET::FIND'

    // Doesn't work
//    bool operator<(const std::unique_ptr<Package>& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//        return this->getName() < package->getName();
////        return this->name < package.getName();
////        return Package::name < package.getName();
////        return Package::name < package.name;
//    }

//    // WORKS for direct comparison without overloading 'std::less' funcion
//    friend bool operator<(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name < anotherPackage->name;
//    }

    // Doesn't work
//    friend bool operator<(std::unique_ptr<Package>& onePackage, std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name < anotherPackage->name;
//    }

// FOR DIRECT (SMART) POINTER COMPARISON FOR 'STD::FIND', 'STD::FIND_IF' (lambda or predicate comparator), 'STD::ANY_OF' (lambda or predicate comparator)

//   WORKS for direct comparison in 'std::find', 'std::find_if', 'std::any_of'
//    friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name == anotherPackage->name;
//    }

    // Doesn't work
//    friend bool operator==(std::unique_ptr<Package>& onePackage, std::unique_ptr<Package>& anotherPackage) {
//        return onePackage->name == anotherPackage->name;
//    }

    // Doesn't work
//    bool operator==(const std::unique_ptr<Package>& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//        return this->getName() == package->getName();
////        return this->name == package.getName();
////        return Package::name == package.getName();
////        return Package::name == package.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR 'SET::FIND'

    // Works for dereferenced comparison together with overloaded 'std::less' funcion for cutom type
    //  or with custom comparator without 'std::less' overload
//    bool operator<(const Package& package) const {
//        // TODO maybe replace the 'getName()' function with only fields?
//        return this->getName() < package.getName();
////        return this->name < package.getName();
////        return Package::name < package.getName();
////        return Package::name < package.name;
//    }

    // Works for dereferenced comparison together with overloaded 'std::less' funcion for cutom type
    //  or with custom comparator without 'std::less' overload
//    friend bool operator<(Package& onePackage, Package& anotherPackage) {
//        return onePackage.name < anotherPackage.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR 'STD::FIND'

    // WORKS only for 'std::find'
//    friend bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    friend bool operator==(std::unique_ptr<Package>& onePackage, Package& anotherPackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    bool operator==(const Package& otherPackage) const {
//        return this->name == otherPackage.name;
//    }

    // Doesn't work - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    bool operator==(Package& otherPackage) {
//        return this->name == otherPackage.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR LAMBDA COMPARATOR OR PREDICATE COMPARATOR FOR 'STD::FIND_IF', 'STD::ANY_OF' WITH SEARCHED ELEMENT PASSED DIRECTLY

    // WORKS
    // Doesn't work for 'std::find'  - error: no match for ‘operator==’ (operand types are ‘const std::unique_ptr<Package>’ and ‘const Package’)
//    friend bool operator==(const Package& onePackage, const Package& anotherPackage) {
//        return onePackage.name == anotherPackage.name;
//    }

    // WORKS for directly passing searched element to the comparator predicate as smart pointer reference
    // Doesn't work for passing searched element to the comparator predicate as dereferenced smart pointer and comparing them directly
    //  - error: passing ‘const Package’ as ‘this’ argument discards qualifiers [-fpermissive]
//    friend bool operator==(Package& onePackage, Package& anotherPackage) {
//        return onePackage.name == anotherPackage.name;
//    }

    // WORKS
//    bool operator==(const Package& otherPackage) const {
//        return this->name == otherPackage.name;
//    }

    // WORKS for directly passing searched element to the comparator predicate as smart pointer reference
    // Doesn't work for passing searched element to the comparator predicate as dereferenced smart pointer and comparing them directly
    //  - error: passing ‘const Package’ as ‘this’ argument discards qualifiers [-fpermissive]
//    bool operator==(Package& otherPackage) {
//        return this->name == otherPackage.name;
//    }

// FOR DEREFERENCED (SMART) POINTER COMPARISON FOR LAMBDA COMPARATOR OR PREDICATE COMPARATOR FOR 'STD::FIND_IF', 'STD::ANY_OF' WITH SEARCHED ELEMENT PASSED AS ALREADY DEREFERENCED

    // WORKS for comparing dereferenced smart pointer (i.e. the instance) with directly passed smart pointer in the comparator predicate
//    friend bool operator==(const Package& anotherPackage, const std::unique_ptr<Package>& onePackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // Doesn't work
    //  - error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘const std::unique_ptr<Package>’) - missing 'const' for all parameters of function
    //  - PackageComparatorPredicate.h:51:33: error: binding reference of type ‘std::unique_ptr<Package>&’ to ‘const std::unique_ptr<Package>’ discards qualifiers
//    friend bool operator==(Package& anotherPackage, std::unique_ptr<Package>& onePackage) {
//        return onePackage->name == anotherPackage.name;
//    }

    // WORKS for comparing dereferenced smart pointer (i.e. the instance) with directly passed smart pointer in the comparator predicate
//    bool operator==(const std::unique_ptr<Package>& otherPackage) const {
//        return this->name == otherPackage->name;
//    }

    // Doesn't work - missing 'const' for parameter
    //  - error: no match for ‘operator==’ (operand types are ‘const Package’ and ‘const std::unique_ptr<Package>’)
    //  - PackageComparatorPredicate.h:51:33: error: binding reference of type ‘std::unique_ptr<Package>&’ to ‘const std::unique_ptr<Package>’ discards qualifiers
//    bool operator==(std::unique_ptr<Package>& otherPackage) {
//        return this->name == otherPackage->name;
//    }

private:
    std::string name;
    std::string locallyInstalledVersion;
    std::string architecture;
    bool isIgnored;

    std::vector<std::unique_ptr<PackageFile>> packageFilesForDeletion;
};

// SPECIALIZED 'STD::LESS' FUNCTOR FOR 'SET::FIND'

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
//            return onePackage < anotherPackage;
//        }
//    };
//}

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
//            return *onePackage < *anotherPackage;
//        }
//    };
//}

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
//namespace std {
//    template<>
//    struct less<unique_ptr<Package>> {
//        bool operator() (const unique_ptr<Package>& onePackage, const unique_ptr<Package>& anotherPackage) const {
//            return onePackage->getName() < anotherPackage->getName();
//        }
//    };
//}
