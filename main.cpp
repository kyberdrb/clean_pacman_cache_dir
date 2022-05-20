#include "Package.h"
#include "IgnoredPackageName.h"
//#include "PackageComparator.h"
#include "PackageComparatorPredicate.h"

#include "alpm.h"
#include "alpm_list.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>

int main() {
    // FIND IGNORED PACKAGES PART - OMMIT/EXCLUDE ALL PACKAGE FILES FROM DELETION THAT MATCH ANY OF THE IGNORED PACKAGE NAMES

    // 'alpm_option_get_ignorepkgs' to retrieve the list of ignored packages from pacman's config doesn't work. Parsing '/etc/pacman.conf' manually
    //alpm_list_t* listOfIgnoredPackages = alpm_option_get_ignorepkgs(handle);

    std::ifstream pacmanConfigFile;

    // TODO parametrize with argument (maybe use getopt?) - if parameter empty, then use default one + check whether the pacman configuration file in the default path actually exists; otherwise exit?/ask user whether to terminate or continue, because the configuration file is used to determine ignored packages in order to exclude them from deletion
    pacmanConfigFile.open("/etc/pacman.conf");

    std::string lineWithIgnoredPackages;
    std::smatch match;
    std::regex regexForIgnoredPackagesInPacmanConfigFile("^IgnorePkg = ");

    while (std::getline(pacmanConfigFile, lineWithIgnoredPackages)) {
        bool doesTheLineContainIgnoredPackages = std::regex_search(lineWithIgnoredPackages, match, regexForIgnoredPackagesInPacmanConfigFile);
        if (doesTheLineContainIgnoredPackages) {
            break;
        }
    }

    // TODO OPTIONAL (assuming no leading spaces/tabs) remove leading and ending blank characters
    // TODO OPTIONAL (assuming no ending spaces/tabs; only one space delimiting [separating] each package filename) replace multiple spaces or tabs with one space
    // build a list of ignored packages

    auto lineWithIgnoredPackagesWithoutOptionPrefix = regex_replace(lineWithIgnoredPackages, regexForIgnoredPackagesInPacmanConfigFile, "");
    std::stringstream ignoredPackagesAsStream{lineWithIgnoredPackagesWithoutOptionPrefix};
    std::string ignoredPackageNameAsText{};
    char delimiterForIgnoredPackages = ' ';

    std::vector<std::string> ignoredPackageNamesInTextFormat{};
    std::vector<std::unique_ptr<IgnoredPackageName>> ignoredPackageNames;

    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPackages)) {
        ignoredPackageNamesInTextFormat.push_back(ignoredPackageNameAsText);

        auto ignoredPackageName = std::make_unique<IgnoredPackageName>(std::move(ignoredPackageNameAsText));
        ignoredPackageNames.emplace_back(std::move(ignoredPackageName));
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF IGNORED PACKAGES\n\n";

    std::cout << "Found " << ignoredPackageNamesInTextFormat.size() << " ignored packages\n\n";

    for (const auto& ignoredPackageName : ignoredPackageNamesInTextFormat) {
        std::cout << ignoredPackageName  << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF IGNORED PACKAGES AS INSTANCES\n\n";

    std::cout << "Found " << ignoredPackageNames.size() << " ignored packages\n\n";

    for (const auto& ignoredPackageName : ignoredPackageNames) {
        std::cout << *ignoredPackageName  << "\n";
    }

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES

    //  Assuming that each package has only one locally installed version with possibly multiple related package files for multiple versions that the Package will remember with itself
    //   and not as a value for the Package key,
    //   therefore a 'set' and
    //    - not a 'multiset' [only one package name with multiple possible versions of it],
    //    - not a 'map' [the values are related and contained in the key itself] and
    //    - not a 'multimap' [the key - package name - is unique - a filesystem feature: each file in a directory has a unique name]
    std::set<std::unique_ptr<Package>> installedPackages{}; // WORKS - with at least overloaded public friend 'operator<' with all const params of reference type to constant unique_ptr to Package

//    std::set<std::unique_ptr<Package, PackageComparator>> installedPackages; // doesn't work - using PackageComparator as a second template argument for 'unique_ptr' as default deleter instead of using it as a second template argument for 'set' as a comparator
//    std::set<std::unique_ptr<Package>, PackageComparator> installedPackages; // WORKS - thanks https://www.codegrepper.com/code-examples/cpp/c%2B%2B+custom+comparator+for+elements+in+set

//    auto packageComparator = std::make_unique<PackageComparator>();
//    std::set<std::unique_ptr<Package>, PackageComparator> installedPackages(*packageComparator); // WORKS

//    auto packageComparatorLambda = [](const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage) {
//        return onePackage < anotherPackage;
//    };
//    std::set<std::unique_ptr<Package>, decltype(packageComparatorLambda)> installedPackages(packageComparatorLambda); // WORKS

//    std::set<std::unique_ptr<Package>, decltype(comparePackages)*> installedPackages(comparePackages); // WORKS
//    std::set<std::unique_ptr<Package>, decltype(comparePackages)*> installedPackages; // Doesn't work - fails at runtime - Works only for C++20 and newer
//    std::set<std::unique_ptr<Package>, decltype(&comparePackages)> installedPackages(&comparePackages); // WORKS
//    std::set<std::unique_ptr<Package>, decltype(&comparePackages)> installedPackages; // Doesn't work - fails at runtime - Works only for C++20 and newer

//    auto packageComparator = std::make_unique<PackageComparator>();
//    std::set<std::unique_ptr<Package>> installedPackages(*packageComparator); // still doesn't work

    alpm_errno_t* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);
    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        std::string packageName = alpm_pkg_get_name(alpm_pkg);
        std::string locallyInstalledVersion = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);

        bool isIgnored = false;
        if(std::find(ignoredPackageNamesInTextFormat.begin(), ignoredPackageNamesInTextFormat.end(), packageName) != ignoredPackageNamesInTextFormat.end()) {
            isIgnored = true;
        }

        bool isIgnoredInAnotherList = false;
        auto packageNameCopy = packageName;
        auto ignoredPackageNameCandidate = std::make_unique<IgnoredPackageName>(std::move(packageNameCopy));
        if(std::find(ignoredPackageNames.begin(), ignoredPackageNames.end(), ignoredPackageNameCandidate) != ignoredPackageNames.end()) {
            isIgnoredInAnotherList = true;
        }

        assert(isIgnored == isIgnoredInAnotherList);

        auto pkg = std::make_unique<Package>(packageName, locallyInstalledVersion, architecture, isIgnored);

        installedPackages.emplace(std::move(pkg));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;

    // FIND PACKAGE FILES IN DIFFERENT PACKAGE VERSIONS FOR SAME PACKAGE,
    //  PARTIALLY DOWNLOADED PACKAGE FILES AND
    //  PACKAGE FILES FOR MISSING PACKAGES

    std::set<std::unique_ptr<PackageFile>> packageFilesRelatedToMissingPackages;
    std::set<std::unique_ptr<PackageFile>> partlyDownloadedPackageFiles;

    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";
    std::filesystem::path aPath {pacmanCacheDir};

    for (const auto& packageFile : std::filesystem::directory_iterator(aPath)) {
        const auto& packageFilenameAsText = packageFile.path().filename().string();
        const auto& packageAbsolutePathAsText = packageFile.path().string();
        const auto& packageFileExtension = packageFile.path().extension().string();

        if (packageFileExtension == ".part") {
            auto partlyDownloadedPackageFile= std::make_unique<PackageFile>(packageAbsolutePathAsText);
            partlyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
            continue;
        }

        // For debugging purposes
//        if (packageFilenameAsText == "libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst") {
//            std::cout << "Here we go..." << "\n";
//        }

        if (packageFile.is_regular_file()) {
            // strip the extensions and architecture from package filename leaving only package name and package version
            //  I couldn't find out how to do first-match or non-greedy replace with regex_replace,
            //  so I'm writing my own algorithm
            auto pkgFilenameAsText = packageFilenameAsText;
            std::reverse(pkgFilenameAsText.begin(), pkgFilenameAsText.end());
            std::stringstream packageNameAndVersionReversed{};
            char delimiter = '-';
            bool stillSearchingForFirstDelimiterOccurence = true;

            //find the first dash '-' in the reversed filename and append everything after
            for (auto character : pkgFilenameAsText) {
                if (character != delimiter && stillSearchingForFirstDelimiterOccurence) {
                    continue;
                }

                stillSearchingForFirstDelimiterOccurence = false;
                packageNameAndVersionReversed << character;
            }

            auto packageNameAndVersion = packageNameAndVersionReversed.str();
            std::reverse(packageNameAndVersion.begin(), packageNameAndVersion.end());
            packageNameAndVersion.pop_back();

            std::string inferredPackageNameAsText = packageNameAndVersion;
            auto packageWithInferredName = std::make_unique<Package>(std::move(inferredPackageNameAsText));

            while ( packageWithInferredName->hasStillSomethingInPackageName() ) {
                // search for the matching package element in the 'installedPackages' by 'packageWithInferredName'
                // 'set::find'
                auto matchingPackage = installedPackages.find(packageWithInferredName);

                // 'std::find' (direct and dereferenced comparison in 'operator==' in class for custom element type)
//                auto matchingPackage = std::find(installedPackages.begin(), installedPackages.end(), packageWithInferredName);  // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
//                auto matchingPackage = std::find(installedPackages.begin(), installedPackages.end(), *packageWithInferredName); // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const Package& anotherPackage)' in 'Package.h'

                // 'std::find_if' with lambda (passing directly) - (direct and dereferenced comparison in lambda)
//                auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
//                        [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
//                            return packageWithInferredName == currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
////                            return *packageWithInferredName == *currentInstalledPackage; // works with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' and 'operator==' const member functions in 'Package.h'
////                            return packageWithInferredName->getName() == currentInstalledPackage->getName();
//                        }
//                );

                // Couldn't make it work
//                auto matchingPackage = std::find_if(installedPackages.begin(), installedPackages.end(),
//                        [&packageWithInferredName](const Package& currentInstalledPackage) {
//                            return packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
////                            return *packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' in 'Package.h'
////                            return packageWithInferredName->getName() == currentInstalledPackage->getName();
//                        }
//                );

                // 'std::find_if' with comparator predicate (directly pass to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                auto matchingPackage = std::find_if(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(packageWithInferredName));

                // 'std::find_if' with comparator predicate (pass dereferenced unique ptr to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                auto matchingPackage = std::find_if(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(*packageWithInferredName));

                // 'std::any_of' with lambda (direct and dereferenced comparison in lambda)
//                bool isPackageWithInferredNameFoundAsTest = std::any_of(installedPackages.begin(), installedPackages.end(),
//                                                                        [&packageWithInferredName](const std::unique_ptr<Package>& currentInstalledPackage) {
////                            return packageWithInferredName == currentInstalledPackage; // works only with 'friend bool operator==(const std::unique_ptr<Package>& onePackage, const std::unique_ptr<Package>& anotherPackage)' in 'Package.h'
////                            return *packageWithInferredName == *currentInstalledPackage; // works only with 'friend bool operator==(const Package& onePackage, const Package& anotherPackage)' in 'Package.h'
//                            return packageWithInferredName->getName() == currentInstalledPackage->getName();
//                        }
//                );

                // 'std::any_of' with comparator predicate (directly pass to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                bool isPackageWithInferredNameFoundAsTest = std::any_of(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(packageWithInferredName));

                // 'std::any_of' with comparator predicate (pass dereferenced unique ptr to comparator predicate - direct and dereferenced comparison in comparator predicate)
//                bool isPackageWithInferredNameFoundAsTest = std::any_of(
//                        installedPackages.begin(),
//                        installedPackages.end(),
//                        PackageComparatorPredicate(*packageWithInferredName));

                // 'std::binary_search'
                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), packageWithInferredName);  // WORKS with overloaded public friend 'operator<' with all const args and even with 'installedPackages' set initialized as 'std::set<std::unique_ptr<Package>> installedPackages{};' with only default comparator. Binary search works only on ordered datastructures. The order is attained by the overloaded 'operator<' as a public friend function with all const params, which gets called at insertion to the 'std::set' - `emplace()`/`push_back()`. That's why the lookup actually finds a matching element of custom type in 'std::set'
//                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), packageWithInferredName, PackageComparator());

//                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), *packageWithInferredName);
//                bool isPackageWithInferredNameFoundAsTest = std::binary_search(installedPackages.begin(), installedPackages.end(), *packageWithInferredName, PackageComparator());

                // For debugging purposes - because the gdb debugger in CLion 2022.1 produces an error when
                //  trying to show the values for STL containers and smartpointer instances.
                //  Instead it shows an error message saying "Cannot instantiate printer for default visualizer"
//                std::cout << *packageWithInferredName << "\n";

                // if key was NOT found, strip the coumpound package key by one character - or word  from the end and perform lookup again
                // comment out for 'std::any_of' and 'std::binary_search'
                bool packageWithInferredNameIsMissing = matchingPackage == installedPackages.end();

                bool packageWithInferredNameIsMissingAsTest = !isPackageWithInferredNameFoundAsTest;
                assert(packageWithInferredNameIsMissing == packageWithInferredNameIsMissingAsTest);

                if (packageWithInferredNameIsMissing) {
                    packageWithInferredName->getNextInferredPackageNameCandidate();
                    continue;
                }

                // if the key WAS found,
                //  - infer the package version from the compound package name and version,
                //  - create a package file with filename, absolute path and package version and add it to the matching locally installed package
                //  - break out of the loop
                auto startingPositionForPackageVersion = packageWithInferredName->getStartingPositionForPackageVersion();
                auto inferredPackageVersionAsText = packageNameAndVersion.substr(startingPositionForPackageVersion);
                auto packageRelatedFile = std::make_unique<PackageFile>(
                        packageFilenameAsText,
                        packageAbsolutePathAsText,
                        std::move(packageWithInferredName->getName()),
                        std::move(inferredPackageVersionAsText) );

                matchingPackage->get()->addPackageFileToDeletionCandidates(std::move(packageRelatedFile));
                break;
            }

            // handle package files for missing reference to locally installed package
            //  i.e. add the package files to another container 'packageFilesForDeletion'
            if (packageWithInferredName->isPackageNameEmpty()) {
                auto packageFileForMissingPackage = std::make_unique<PackageFile>(packageAbsolutePathAsText);
                packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF INSTALLED PACKAGES WITH RELATED PACKAGE FILES FOR DIFFERENT VERSIONS\n\n";

    std::cout << "Found " << installedPackages.size() << " installed packages\n\n";

    for (const auto& package : installedPackages) {
        std::cout << *package << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PARTLY DOWNLOADED PACKAGE FILES\n\n";

    std::cout << "Found " << partlyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : partlyDownloadedPackageFiles) {
        std::cout << *partlyDownloadedPackageFile << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PACKAGE FILES RELATED TO MISSING PACKAGES\n\n";

    std::cout << "Found " << packageFilesRelatedToMissingPackages.size() << " package files related to missing packages\n\n";

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        std::cout << *packageFilesRelatedToMissingPackage << "\n";
    }

    // MOVING PACKAGE FILES TO SEPARATE DIRECTORY PART

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "MOVING PACKAGES\n\n";

    std::string pathToDuplicateFilesDirectoryAsText =
            pacmanCacheDir + "/PACKAGE_FILES_FOR_VERSIONS_OTHER_THAN_LOCALLY_INSTALLED/";
    std::filesystem::create_directories(pathToDuplicateFilesDirectoryAsText);

    for (const auto& installedPackage: installedPackages) {
        installedPackage->movePackageFilesForDifferentVersionsToSeparateDir(pathToDuplicateFilesDirectoryAsText);
    }

    for (const auto& partlyDownloadedPackageFile: partlyDownloadedPackageFiles) {
            const std::string& from = partlyDownloadedPackageFile->getAbsolutePath();
            const std::string& to = pathToDuplicateFilesDirectoryAsText +
                    partlyDownloadedPackageFile->getFilename();
            std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }

    for (const auto& packageFilesRelatedToMissingPackage: packageFilesRelatedToMissingPackages) {
        const std::string& from = packageFilesRelatedToMissingPackage->getAbsolutePath();
        const std::string& to = pathToDuplicateFilesDirectoryAsText +
                packageFilesRelatedToMissingPackage->getFilename();
        std::cout << "Moving package file\t\t" << from << "\nto separate directory\t" << to << "\n\n";
            std::filesystem::rename(from, to);
    }

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    return 0;
}
