//
// Created by laptop on 6/19/22.
//

#include "MatchFinderForPackageFilesToLocallyInstalledPackages.h"

#include "PackageNameMissing.h"
#include "PackageWithInferredName.h"
#include "TerminalSingleton.h"

#include <filesystem>
#include <memory>
#include <set>

// libs to detect the home directory path of the current user, even behind 'sudo'
//#include <csignal> // for 'getuid()' to get the user ID (UID) for 'getpwuid()' - offered as auto-include by CLion - preffered, because it produces smaller debug binary by 224 B
#include <unistd.h> // for 'getuid()' to get the user ID (UID) for 'getpwuid()' - included in examples from people on the internet
#include <pwd.h> // for 'getpwuid()' to get the home directory for the given UID
#include <libaudit.h> // for 'audit_getloginuid()' to detect the UID of the user who invoked 'sudo', instead of the 'root' user

// For debugging purposes
//#include "TerminalSingleton.h"

MatchFinderForPackageFilesToLocallyInstalledPackages::MatchFinderForPackageFilesToLocallyInstalledPackages(
        LocallyInstalledPackages& locallyInstalledPackages)
:
        locallyInstalledPackages(locallyInstalledPackages)
{
    relatePackageFilesToLocallyInstalledPackages();
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::relatePackageFilesToLocallyInstalledPackages() {
    // TODO iterate through multiple directories with installation package files, not only within the default pacman's cache dir "/var/cache/pacman/pkg"
    //  but also in pikaur cache directories:

    // TODO share one copy of 'pacmanCacheDir' across all instances that uses it
//    auto pacmanCacheDir = std::make_unique<AbsolutePath>("/var/cache/pacman/pkg/");
//    this->relatePackageFilesToLocallyInstalledPackagesForDirectory(*pacmanCacheDir);

    auto pikaurCacheDirSystem = std::make_unique<AbsolutePath>("/var/cache/pikaur/pkg/");
    this->relatePackageFilesToLocallyInstalledPackagesForDirectory(*pikaurCacheDirSystem);

    // TODO find out the home directory of current user programmatically; something like
    //     "${HOME}/.cache/pikaur/pkg/"
    //  in shell
//    auto pikaurCacheDirUser = std::make_unique<AbsolutePath>("/home/laptop/.cache/pikaur/pkg/");

    std::stringstream pikaurCacheDirUserAsStream;

    const char* currentUserHomeDirCheck = NULL;
    currentUserHomeDirCheck = std::getenv("HOME");
    currentUserHomeDirCheck = std::getenv("HOMEE");
    currentUserHomeDirCheck = std::getenv("HOME");

    std::string currentUserHomeDir{};
    if (currentUserHomeDirCheck != NULL) {
        currentUserHomeDir = std::getenv("HOME");
        TerminalSingleton::get().printText(currentUserHomeDir);
        TerminalSingleton::get().printText("\n");
    }

    //if (currentUserHomeDirCheck == NULL) {
        // the '$HOME' variable is not defined in the system - using fallback
        currentUserHomeDir = getpwuid(getuid())->pw_dir;
        TerminalSingleton::get().printText(currentUserHomeDir);
        TerminalSingleton::get().printText("\n");

        currentUserHomeDir = getpwuid(geteuid())->pw_dir;
        TerminalSingleton::get().printText(currentUserHomeDir);
        TerminalSingleton::get().printText("\n");

        currentUserHomeDir = getpwuid(audit_getloginuid())->pw_dir;
        TerminalSingleton::get().printText(currentUserHomeDir);
        TerminalSingleton::get().printText("\n");

        std::exit(1);
    //}

    pikaurCacheDirUserAsStream << currentUserHomeDir;
    pikaurCacheDirUserAsStream << "/.cache/pikaur/pkg/";

    auto pikaurCacheDirUser = std::make_unique<AbsolutePath>(pikaurCacheDirUserAsStream.str());
    this->relatePackageFilesToLocallyInstalledPackagesForDirectory(*pikaurCacheDirUser);
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::relatePackageFilesToLocallyInstalledPackagesForDirectory(
        const AbsolutePath& directoryWithInstallationPackageFiles)
{
    std::filesystem::path pacmanCacheDirPath {directoryWithInstallationPackageFiles.getAbsolutePath()};

    try {
        std::filesystem::exists(pacmanCacheDirPath);
    } catch (const std::filesystem::__cxx11::filesystem_error& exception) {
        std::stringstream message;

        message
                << exception.what() << "\n"
                << "Skipping scanning of directory: " << pacmanCacheDirPath << "\n"
                << "Error: Insufficient permissions to access the files in the directory or file path doesn't exist." << "\n"
                << "Please, run this program with elevated priviledges as 'sudo' or 'root' user"
                   " and make sure the source file is present on the filesystem."
                << "\n---\n";

        TerminalSingleton::get().printText(message.str());

        return;
    }

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

            this->partiallyDownloadedPackageFiles.emplace(std::move(partlyDownloadedPackageFile));
            continue;
        }

        // For debugging purposes
//        if (packageFilenameAsText == "libyuv-r2266+eb6e7bb6-1-x86_64.pkg.tar.zst") {
//            TerminalSingleton::get().printText("Here we go...\n");
//        }

        if (packageFile.is_regular_file()) {
            std::string inferredPackageNameAsText = packageFilename->extractPackageNameAndVersion();

            std::unique_ptr<Package> packageWithInferredName =
                    std::make_unique<PackageWithInferredName>(std::move(inferredPackageNameAsText));

            auto* packageWithInferredNameExact = dynamic_cast<PackageWithInferredName*>(packageWithInferredName.get());

            // For debugging purposes
//            assert(packageWithInferredNameExact != nullptr);

            while ( packageWithInferredNameExact->hasStillSomethingInPackageName() ) {
                try {
                    // search for the matching package element in the 'locallyInstalledPackages' by 'packageWithInferredName'
                    const Package& matchingLocallyInstalledPackage = this->locallyInstalledPackages.find(packageWithInferredName);

                    // if the key WAS found,
                    //  - infer the package version from the compound package name and version,
                    //  - create a package file with filename, absolute path and package version
                    //     and add it to the matching locally installed package
                    //  - break out of the loop

                    // For debugging purposes
//                    assert(matchingLocallyInstalledPackage.getName().string() == packageWithInferredName->getName().string());

                    auto inferredPackageVersion = packageWithInferredNameExact->extractPackageVersion();

                    auto packageRelatedFile = std::make_unique<ExtendedInstallationPackageFile>(
                            std::move(packageAbsolutePath),
                            std::move(packageFilename),
                            matchingLocallyInstalledPackage.getName(),
                            std::move(inferredPackageVersion));

                    const LocallyInstalledPackage& locallyInstalledPackageExact =
                            *( dynamic_cast<const LocallyInstalledPackage*>(&matchingLocallyInstalledPackage) );

                    // For debugging purposes
                    // assert(locallyInstalledPackageExact != nullptr);

                    LocallyInstalledPackage& localyInstalledPackageExactModifiable =
                            *( const_cast<LocallyInstalledPackage*>(&locallyInstalledPackageExact) );

                    // For debugging purposes
                    //  KNOWN BUG: algorithm matches installation package file 'gdb-frontend-gui' to package 'gdb'
                    //   instead of adding it to the collection of missing packages when no 'gdb-frontend-gui' is present in the system
                    //   but the behavior of deleting the package file, because its version ["frontend-bin-0.11.2.beta-1"]
                    //   was lower than the one of 'gdb' ["12.1-1"]
                    //   but other cases I didn't test, and I don't know what will happen, when the version will have different composition
//                    if (matchingLocallyInstalledPackage.getName().string() == "gdb") {
//                        TerminalSingleton::get().printText("Found suspicious package");
//                    }

                    bool wasInstallationPackageFileAdded =
                            localyInstalledPackageExactModifiable.addPackageFileToDeletionCandidatesOnlyWhenMatchingCriteria(
                                    std::move(packageRelatedFile));

                    // if the package file was added to the deletion candidates for the particular package,
                    //  save the reference to the package file for generating only
                    //  and faster deleting of the package files by iterating only packages that have at least one package file for deletion
                    if (wasInstallationPackageFileAdded) {
                        this->locallyInstalledPackages.addReferenceToPackageRelatedToInstallationPackageFileForDifferentVersion(
                                localyInstalledPackageExactModifiable);
                    }

                    break;
                } catch (const PackageNameMissing& exception) {
                    // if key was NOT found, strip the compound package key from the end by one word up to the next delimiter
                    //  and perform lookup again

                    // For debugging purposes - because the gdb debugger in CLion 2022.1 produces an error when
                    // trying to show the values for STL containers and smartpointer instances.
                    // Instead, it shows an error message saying "Cannot instantiate printer for default visualizer"
//                    std::stringstream message;
//                    message << exception.what() << "\n";
//                    TerminalSingleton::get().printText(message.str());

                    packageWithInferredNameExact->getNextInferredPackageNameCandidate();
                    continue;
                }
            }

            bool hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage = packageWithInferredNameExact->isPackageNameEmpty();

            if (hasInstallationPackageFileMissingReferenceToLocallyInstalledPackage) {
                auto packageFileForMissingPackage = std::make_unique<SimpleInstallationPackageFile>(
                        std::move(packageAbsolutePath),
                        std::move(packageFilename),
                        SimpleInstallationPackageFileType::MISSING_LOCALLY_INSTALLED_PACKAGE);

                this->packageFilesRelatedToMissingPackages.emplace(std::move(packageFileForMissingPackage));
            }
        }
    }
}

std::string MatchFinderForPackageFilesToLocallyInstalledPackages::generateReport() const {
    std::stringstream report;

    report << "\n";
    report << "===============================================\n\n";
    report << "LIST OF PARTIALLY DOWNLOADED INSTALLATION PACKAGE FILES\n\n";

    report << "Found " << this->partiallyDownloadedPackageFiles.size() << " partly downloaded package files\n\n";

    for (const auto& partlyDownloadedPackageFile : this->partiallyDownloadedPackageFiles) {
        report << *partlyDownloadedPackageFile << "\n";
    }

    report << "\n";
    report << "===============================================\n\n";
    report << "LIST OF INSTALLATION PACKAGE FILES RELATED TO MISSING PACKAGES\n\n";

    report << "Found " << this->packageFilesRelatedToMissingPackages.size() << " package files related to missing packages\n\n";

    for (const auto& packageFilesRelatedToMissingPackage : this->packageFilesRelatedToMissingPackages) {
        report << *packageFilesRelatedToMissingPackage << "\n";
    }
    
    return report.str();
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::moveChosenInstallationPackageFiles(
        const AbsolutePath& directoryForInstallationPackageFilesForDeletion) const
{
    for (const auto& partiallyDownloadedPackageFile : this->partiallyDownloadedPackageFiles) {
        partiallyDownloadedPackageFile->moveToSeparateDirectoryForDeletion(directoryForInstallationPackageFilesForDeletion);
    }

    for (const auto& packageFilesRelatedToMissingPackage : this->packageFilesRelatedToMissingPackages) {
        packageFilesRelatedToMissingPackage->moveToSeparateDirectoryForDeletion(directoryForInstallationPackageFilesForDeletion);
    }

    // TODO completely clean all file within all subdirs within pikaur cache directory `/var/cache/pikaur`  which likely references to `/var/cache/private/pikaur` (only accessible with superuser/sudo/root) priviledges
    //  by not deleting the pikaur directories themselves, but by deleting all files within the pikaur directories

    // TODO completely clean all files ** only ** within the pikaur cache directory `"${HOME}/.cache/pikaur/"`

    // TODO completely clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/build/"`

    // TODO ** selectively ** clean all contents within the pikaur cache directory `"${HOME}/.cache/pikaur/pkg/"
}