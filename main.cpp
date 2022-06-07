#include "IgnoredPackageName.h"
#include "Package.h"
#include "SimpleInstallationPackageFile.h"

#include "alpm.h"
#include "alpm_list.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>

//#include <cassert>

#include "Package__refactored__.h"
#include "PackageWithInferredName__refactored__.h"
#include "LocallyInstalledPackage__refactored__.h"
void testPolymorphism() {
    auto locallyInstalledPackageAuto = std::make_unique<LocallyInstalledPackage__refactored__>();
    locallyInstalledPackageAuto->commonFunction();
    locallyInstalledPackageAuto->functionOnlyInLocallyInstalledPackage__refactored__();

    std::cout << "---\n";

    std::unique_ptr<LocallyInstalledPackage__refactored__> locallyInstalledPackageExact = std::make_unique<LocallyInstalledPackage__refactored__>();
    locallyInstalledPackageExact->commonFunction();
    locallyInstalledPackageExact->functionOnlyInLocallyInstalledPackage__refactored__();

    std::cout << "---\n";

    std::unique_ptr<Package__refactored__> locallyInstalledPackageAbstract = std::make_unique<LocallyInstalledPackage__refactored__>();
    locallyInstalledPackageAbstract->commonFunction();
//    locallyInstalledPackageAbstract->functionOnlyInLocallyInstalledPackage__refactored__(); // Compilation error: No member named 'functionOnlyInLocallyInstalledPackage__refactored__' in 'Package__refactored__'

    std::cout << "===\n";

    auto packageWithInferredNameAuto = std::make_unique<PackageWithInferredName__refactored__>();
    packageWithInferredNameAuto->commonFunction();
    packageWithInferredNameAuto->functionOnlyInPackageWithInferredName__refactored__();

    std::cout << "---\n";

    std::unique_ptr<PackageWithInferredName__refactored__> packageWithInferredNameExact = std::make_unique<PackageWithInferredName__refactored__>();
    packageWithInferredNameExact->commonFunction();
    packageWithInferredNameExact->functionOnlyInPackageWithInferredName__refactored__();

    std::cout << "---\n";

    std::unique_ptr<Package__refactored__> packageWithInferredNameGeneralized = std::make_unique<PackageWithInferredName__refactored__>();
    packageWithInferredNameGeneralized->commonFunction();
//    packageWithInferredNameGeneralized->functionOnlyInPackageWithInferredName__refactored__(); // Compilation error: No member named 'functionOnlyInLocallyInstalledPackage__refactored__' in 'Package__refactored__'
}

int main() {
    testPolymorphism();

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

    std::vector<std::unique_ptr<IgnoredPackageName>> ignoredPackageNames;

    while(getline(ignoredPackagesAsStream, ignoredPackageNameAsText, delimiterForIgnoredPackages)) {
        auto ignoredPackageName = std::make_unique<IgnoredPackageName>(std::move(ignoredPackageNameAsText));
        ignoredPackageNames.emplace_back(std::move(ignoredPackageName));
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF IGNORED PACKAGES\n\n";

    std::cout << "Found " << ignoredPackageNames.size() << " ignored packages\n\n";

    for (const auto& ignoredPackageName : ignoredPackageNames) {
        std::cout << *ignoredPackageName  << "\n";
    }

    // BUILD LIST OF LOCALLY INSTALLED PACKAGES

    // Assuming that each package has only one locally installed version with possibly multiple related package files for
    //  multiple versions that the Package will remember within itself
    //  and not as a value for the Package key.
    // "Some sets store object that embed their own keys, that is to say that such objects have a subpart that is to be considered as
    //  a key, like an ID for example, while the object itself is to be considered as a value."
    //   - https://www.fluentcpp.com/2017/06/09/search-set-another-type-key/
    //  Therefore a 'set' and
    //   - not a 'multiset' [the embedded key - package name - is unique - only one package name in all different versions of it
    //     than the local one],
    //   - not a 'map' [the values are related and contained in the key itself] and
    //   - not a 'multimap' [the key - package name - is unique - a filesystem feature: each file in a directory has a unique name]
    std::set<std::unique_ptr<Package>> installedPackages{};
//    std::set<std::unique_ptr<Package>, PackageComparator> installedPackages{};
//    std::set<std::unique_ptr<Package>, std::greater<std::unique_ptr<Package>>> installedPackages{};

    // Transparent functor 'std::greater' didn't work: no element was found
//    std::set<std::unique_ptr<Package>, std::greater<>> installedPackages{};

    alpm_errno_t* err = reinterpret_cast<alpm_errno_t*>(calloc(1, sizeof(alpm_errno_t)));
    alpm_handle_t* handle = alpm_initialize("/", "/var/lib/pacman/", err);
    alpm_db_t* db_local = alpm_get_localdb(handle);
    alpm_list_t* listOfAllLocallyInstalledPackages = alpm_db_get_pkgcache(db_local);

    while (listOfAllLocallyInstalledPackages != NULL) {
        alpm_pkg_t* alpm_pkg = reinterpret_cast<alpm_pkg_t*>(listOfAllLocallyInstalledPackages->data);
        listOfAllLocallyInstalledPackages = alpm_list_next(listOfAllLocallyInstalledPackages);

        // TODO instead of copying and moving 'packageNameAsText' to instances of 'Package' class
        // (or derived 'Package' classes?)
        //  create 'PackageName' instances by moving 'packageNameAsText' into it,
        //  store 'PackageName' instances in a collection ('std::set'?)
        //  or a temporary variable? (std::unique_ptr<Filename>)
        //  and then only copy reference to specific package to various 'Package' instances
        std::string packageNameAsText = alpm_pkg_get_name(alpm_pkg);

        std::string locallyInstalledVersionAsText = alpm_pkg_get_version(alpm_pkg);
        std::string architecture = alpm_pkg_get_arch(alpm_pkg);

        bool isIgnored = false;
        auto ignoredPackageNameCandidate = std::make_unique<IgnoredPackageName>(packageNameAsText);

        // For debugging purposes - if the argument is passed by value to a function, which accepts the argument as a value
        //  the argument is __copied__ from the calling to the receiving function
//        assert(packageNameAsText != "");

        if( std::find(ignoredPackageNames.begin(), ignoredPackageNames.end(), ignoredPackageNameCandidate) != ignoredPackageNames.end() ) {
            isIgnored = true;
        }

        auto packageName = std::make_unique<PackageName>(std::move(packageNameAsText));

        // For debugging purposes - if the argument is passed by value with 'std::move' to a function, which accepts the argument as a value
        //  the argument is __moved__ from the calling to the receiving function
//        assert(packageNameAsText == "");

        auto locallyInstalledVersion = std::make_unique<PackageVersion>(std::move(locallyInstalledVersionAsText));

        auto locallyInstalledPackage = std::make_unique<Package>(std::move(packageName), std::move(locallyInstalledVersion), architecture, isIgnored);
        // TODO refactor to construct 'LocallyInstalledPackage' unique pointer
//        auto locallyInstalledPackage = std::make_unique<LocallyInstalledPackage>(std::move(packageName), std::move(locallyInstalledVersion), architecture, isIgnored);

        installedPackages.emplace(std::move(locallyInstalledPackage));
    }

    free(err);
    err = nullptr;

    alpm_release(handle);
    handle = nullptr;

    // FIND PACKAGE FILES IN DIFFERENT PACKAGE VERSIONS FOR SAME PACKAGE,
    //  PARTIALLY DOWNLOADED PACKAGE FILES AND
    //  PACKAGE FILES FOR MISSING PACKAGES

    std::set<std::unique_ptr<SimpleInstallationPackageFile>> packageFilesRelatedToMissingPackages;
    std::set<std::unique_ptr<SimpleInstallationPackageFile>> partiallyDownloadedPackageFiles;

    std::set<std::reference_wrapper<Package>> packagesWithInstallationPackageFilesForDifferentVersions;
    // TODO refactor to hold 'LocallyInstalledPackage' reference_wrappers
//    std::set<std::reference_wrapper<LocallyInstalledPackage>> packagesWithInstallationPackageFilesForDifferentVersions;

    const std::string pacmanCacheDir = "/var/cache/pacman/pkg";
    std::filesystem::path pacmanCacheDirPath {pacmanCacheDir};

    for (const auto& packageFile : std::filesystem::directory_iterator(pacmanCacheDirPath)) {
        const auto& packageFileExtension = packageFile.path().extension().string();

        const auto& packageAbsolutePathAsText = packageFile.path().string();
        auto packageAbsolutePath = std::make_unique<AbsolutePath>(std::move( *(const_cast<std::string*>(&packageAbsolutePathAsText) ) ) );

        const auto& packageFilenameAsText = packageFile.path().filename().string();
        auto packageFilename = std::make_unique<Filename>(std::move( *(const_cast<std::string*>(&packageFilenameAsText) ) ) );

        if (packageFileExtension == ".part") {
            auto partlyDownloadedPackageFile= std::make_unique<SimpleInstallationPackageFile>(
                    std::move(packageAbsolutePath),
                    std::move(packageFilename),
                    SimpleInstallationPackageFileType::PARTIALLY_DOWNLOADED);

            partiallyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
            continue;
        }

        // For debugging purposes
//        if (packageFilenameAsText == "libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst") {
//            std::cout << "Here we go..." << "\n";
//        }

        if (packageFile.is_regular_file()) {
            auto packageNameAndVersionAsText = packageFilename->extractPackageNameAndVersion();
            std::string inferredPackageNameAsText = packageNameAndVersionAsText;
            auto inferredPackageName = std::make_unique<PackageName>(std::move(inferredPackageNameAsText));

            auto packageWithInferredName = std::make_unique<Package>(std::move(inferredPackageName));
            // TODO refactor to use 'PackageWithInferredName' - the derived class of 'Package' base class
//            auto packageWithInferredName = std::make_unique<PackageWithInferredName>(std::move(inferredPackageName));

            while ( packageWithInferredName->hasStillSomethingInPackageName() ) {
                // search for the matching package element in the 'installedPackages' by 'packageWithInferredName'
                auto iteratorPointingToMatchingPackage = installedPackages.find(packageWithInferredName);

                // For debugging purposes - because the gdb debugger in CLion 2022.1 produces an error when
                //  trying to show the values for STL containers and smartpointer instances.
                //  Instead, it shows an error message saying "Cannot instantiate printer for default visualizer"
//                std::cout << *packageWithInferredName << "\n";

                // if key was NOT found, strip the coumpound package key by one character - or word  from the end and perform lookup again
                bool packageWithInferredNameIsMissing = iteratorPointingToMatchingPackage == installedPackages.end();
                if (packageWithInferredNameIsMissing) {
                    packageWithInferredName->getNextInferredPackageNameCandidate();
                    continue;
                }

                // if the key WAS found,
                //  - infer the package version from the compound package name and version,
                //  - create a package file with filename, absolute path and package version
                //     and add it to the matching locally installed package
                //  - break out of the loop

                // For debugging purposes
//                assert(iteratorPointingToMatchingPackage->get()->getName().string() == packageWithInferredName->getName().getAbsolutePath());

                auto startingPositionForPackageVersion = packageWithInferredName->getStartingPositionForPackageVersion();
                auto inferredPackageVersionAsText = packageNameAndVersionAsText.substr(startingPositionForPackageVersion);
                auto inferredPackageVersion = std::make_unique<PackageVersion>(inferredPackageVersionAsText);

                auto packageRelatedFile = std::make_unique<ExtendedInstallationPackageFile>(
                        std::move(packageAbsolutePath),
                        std::move(packageFilename),
                        iteratorPointingToMatchingPackage->get()->getName(),
                        std::move(inferredPackageVersion));

                bool wasInstallationPackageFileAdded = iteratorPointingToMatchingPackage->get()->addPackageFileToDeletionCandidates(std::move(packageRelatedFile));

                // if the package file was added to the deletion candidates for the particular package,
                //  save the reference to the package file for generating only
                //  and faster deleting of the package files by iterating only packages that have at least one package file for deletion
                if (wasInstallationPackageFileAdded) {
                    packagesWithInstallationPackageFilesForDifferentVersions.emplace(*(iteratorPointingToMatchingPackage->get()));
                }

                break;
            }

            bool hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage = packageWithInferredName->isPackageNameEmpty();

            if (hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage) {
                auto packageFileForMissingPackage = std::make_unique<SimpleInstallationPackageFile>(
                        std::move(packageAbsolutePath),
                        std::move(packageFilename),
                        SimpleInstallationPackageFileType::MISSING_LOCALLY_INSTALLED_PACKAGE);

                packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF ALL INSTALLED PACKAGES WITH RELATED PACKAGE FILES FOR DIFFERENT VERSIONS (IF ANY)\n\n";

    std::cout << "Found " << installedPackages.size() << " installed packages\n\n";

    for (const auto& package : installedPackages) {
        std::cout << *package << "\n";
    }

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF INSTALLED PACKAGES THAT HAVE AT LEAST ONE RELATED INSTALLATION PACKAGE FILE FOR DIFFERENT VERSION THAN THE LOCALLY INSTALLED ONE\n\n";

    std::cout << "Found " << packagesWithInstallationPackageFilesForDifferentVersions.size()
        << " installed packages with installation package files for other than locally installed version\n\n";

    uint_fast16_t numberOfInstallationPackageFilesForOtherVersions = 0;

    for (const auto package : packagesWithInstallationPackageFilesForDifferentVersions) {
        std::cout << package << "\n";

        uint_fast16_t numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage =
            package.get().getNumberOfInstallationPackageFilesForDifferentVersions();

        numberOfInstallationPackageFilesForOtherVersions += numberOfInstallationPackageFilesForDifferentVersionsForCurrentPackage;
    }

    std::cout << "\n";
    std::cout << "Found " << numberOfInstallationPackageFilesForOtherVersions
        << " installation package files for different version than the locally installed\n";

    std::cout << "\n";
    std::cout << "===============================================\n\n";
    std::cout << "LIST OF PARTLY DOWNLOADED PACKAGE FILES\n\n";

    std::cout << "Found " << partiallyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : partiallyDownloadedPackageFiles) {
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

    auto directoryForInstallationPackageFilesForDeletion = std::make_unique<AbsolutePath>(pathToDuplicateFilesDirectoryAsText);

    for (const auto& installedPackage : packagesWithInstallationPackageFilesForDifferentVersions) {
        installedPackage.get().movePackageFilesForDifferentVersionsToSeparateDir( *(directoryForInstallationPackageFilesForDeletion) );
    }

    for (const auto& partiallyDownloadedPackageFile : partiallyDownloadedPackageFiles) {
        partiallyDownloadedPackageFile->moveToSeparateDirectoryForDeletion(*(directoryForInstallationPackageFilesForDeletion));
    }

    for (const auto& packageFilesRelatedToMissingPackage : packageFilesRelatedToMissingPackages) {
        packageFilesRelatedToMissingPackage->moveToSeparateDirectoryForDeletion(*(directoryForInstallationPackageFilesForDeletion));
    }

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    // TODO completely clean all files ** only ** within the pikaur cache directory `"${HOME}/.cache/pikaur/"`

    // TODO completely clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/build/"`

    // TODO ** selectively ** clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/pkg/"

    return 0;
}
