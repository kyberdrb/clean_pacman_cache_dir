//
// Created by laptop on 6/19/22.
//

#include "MatchFinderForPackageFilesToLocallyInstalledPackages.h"

#include "FileMoverSingleton.h"
#include "PackageNameMissing.h"
#include "PackageWithInferredName.h"
#include "TerminalAndLoggerSingleton.h"

#include <filesystem>
#include <memory>
#include <set>

// helpers to detect the home directory path of the current user, even behind 'sudo'
#include <libaudit.h> // for 'audit_getloginuid()' to detect the UID of the user who invoked 'sudo', instead of the 'root' user
#include <pwd.h> // for 'getpwuid()' to get the home directory for the given UID

MatchFinderForPackageFilesToLocallyInstalledPackages::MatchFinderForPackageFilesToLocallyInstalledPackages(
        LocallyInstalledPackages& locallyInstalledPackages)
:
        locallyInstalledPackages(locallyInstalledPackages),
        // the home directory detection can still fail when the user had been created without home directory
        currentUserHomeDir(getpwuid(audit_getloginuid())->pw_dir)
{
    this->relateInstallationPackageFilesToLocallyInstalledPackagesForAllCacheDirs();
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::relateInstallationPackageFilesToLocallyInstalledPackagesForAllCacheDirs() {
    // TODO add paths into a 'std::vector' (e.g. by calling a function designated for this purpose in the constructor)
    //  and recompute the installation package files to locally installed packages relationships in one for-each loop (e.g. in )

    // Iterate cache directory for pacman
    // TODO share one copy of 'pacmanCacheDir' across all instances that uses it, and reference it with other more accurate variable names
    //  when used for different reasons
    auto pacmanCacheDir = std::make_unique<AbsolutePath>("/var/cache/pacman/pkg/");
    this->relatePackageFilesToLocallyInstalledPackagesForDirectory(*pacmanCacheDir);

    // Iterate system cache directory for pikaur
    auto pikaurCacheDirSystem = std::make_unique<AbsolutePath>("/var/cache/pikaur/pkg/");
    this->relatePackageFilesToLocallyInstalledPackagesForDirectory(*pikaurCacheDirSystem);

    // Iterate user cache directory for pikaur
    auto pikaurCacheDirUser = std::make_unique<AbsolutePath>(this->determinePikaurCacheDirUser());
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
                   " and make sure the source file is present on the filesystem." << "\n"
                << "---\n";

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
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
//            TerminalAndLoggerSingleton::get().printAndLog("Here we go...\n");
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
                    //  KNOWN BUG: installation package file for package 'gdb-frontend-gui' is assigned to locally installed package 'gdb',
                    //   instead of addied to the collection of installation package files for missing packages
                    //   because no 'gdb-frontend-gui' package is not present, i.e. missing in the system.
                    //   But the behavior of deleting the package file, because its version was different is correct. Because the
                    //   incorrectly related installation package file has different version than the locally installed one,
                    //   the installation package file will be still added to the deletion candidates for the locally installed package:
                    //   REAL VERSION COMPARISON SCENARIO
                    //       [gdb]         [gdb-frontend-gui]
                    //       ["12.1-1"]    ["frontend-bin-0.11.2.beta-1"]
                    //   HYPOTHETICAL VERSION COMPARISON SCENARIO
                    //       [gdb]         [gdb-frontend-gui]
                    //       ["12.1-1"]    ["frontend-bin-12.1-1"]
                    //   Even when the versions of both packages would match, the extracted package version for the unrelated installation
                    //   package file would be partial and thus prepended with the rest of the left-over filename of the installation
                    //   package file, thus still being marked as deletion candidate for the locally installed package.
                    //   See function 'addPackageFileToDeletionCandidatesOnlyWhenMatchingCriteria' in 'LocallyInstalledPackage'
                    //   to check the criteria used when adding a package file to the deletion candidates in
                    //   'installationPackageFilesForDifferentPackageVersions'
//                    if (matchingLocallyInstalledPackage.getName().string() == "gdb") {
//                        TerminalAndLoggerSingleton::get().printAndLog("Found suspicious package");
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
//                    TerminalAndLoggerSingleton::get().printAndLog(message.str());

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

std::string MatchFinderForPackageFilesToLocallyInstalledPackages::determinePikaurCacheDirUser() {
    std::stringstream pikaurCacheDirUserAsStream;

    // TODO centralize duplicate code by encapsulating the home dir detection to a separate class
    //  and use it here and in 'MatchFinderForPackageFilesToLocallyInstalledPackages.cpp'

    pikaurCacheDirUserAsStream << this->currentUserHomeDir;
    pikaurCacheDirUserAsStream << "/.cache/pikaur/pkg/";
    return pikaurCacheDirUserAsStream.str();
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
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::cleanUpOtherFilesInPikaurCacheDirs(
        const AbsolutePath& destinationDirectory) const
{
    std::stringstream pikaurUserCacheDirAsStream{};
    pikaurUserCacheDirAsStream << this->currentUserHomeDir << "/.cache/pikaur/";
    auto pikaurUserCacheDir = std::make_unique<AbsolutePath>(pikaurUserCacheDirAsStream.str());
    this->moveOnlyFilesFromDir(*pikaurUserCacheDir, destinationDirectory);

    auto pikaurUserCacheDirBuildDir {std::make_unique<AbsolutePath>("/home/laptop/.cache/pikaur/build/")};
    this->moveEverythingFromDir(*pikaurUserCacheDirBuildDir, destinationDirectory);

    auto pikaurSystemCacheDir = std::make_unique<AbsolutePath>("/var/cache/pikaur");
    this->moveOnlyFilesFromDir(*pikaurSystemCacheDir, destinationDirectory);

    auto pikaurSystemCacheDirAurReposDir {std::make_unique<AbsolutePath>("/var/cache/private/pikaur/aur_repos/")};
    this->moveEverythingFromDir(*pikaurSystemCacheDirAurReposDir, destinationDirectory);

    auto pikaurSystemCacheDirBuildDir {std::make_unique<AbsolutePath>("/var/cache/private/pikaur/build/")};
    this->moveEverythingFromDir(*pikaurSystemCacheDirBuildDir, destinationDirectory);
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::moveOnlyFilesFromDir(
        const AbsolutePath& absolutePathToSourceDirectoryAsText,
        const AbsolutePath& absolutePathToDestinationDirectoryAsText) const
{
    std::filesystem::path pathToSourceDirectory {absolutePathToSourceDirectoryAsText.getAbsolutePath()};

    try {
        std::filesystem::exists(pathToSourceDirectory);
    } catch (const std::filesystem::__cxx11::filesystem_error& exception) {
        std::stringstream message;
        message
                << exception.what() << "\n"
                << "Skipping scanning of directory: " << pathToSourceDirectory << "\n"
                << "Error: Insufficient permissions to access the files in the directory or file path doesn't exist." << "\n"
                << "Please, run this program with elevated priviledges as 'sudo' or 'root' user"
                   " and make sure the source file is present on the filesystem." << "\n"
                << "---\n";

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
        return;
    }

    for (const auto& fileInSourceDirectory : std::filesystem::directory_iterator(pathToSourceDirectory)) {
        std::stringstream message{};

        if (fileInSourceDirectory.is_regular_file()) {
            auto pathToFileInSourceDirectory = std::make_unique<AbsolutePath>(fileInSourceDirectory.path().string());

            std::stringstream destinationAbsolutePathAsStream;
            destinationAbsolutePathAsStream
                << absolutePathToDestinationDirectoryAsText
                << fileInSourceDirectory.path().filename().string();
            auto destinationAbsolutePath = std::make_unique<AbsolutePath>(destinationAbsolutePathAsStream.str());

            message << "Moving file from source path:\n"
                    << "    " << *pathToFileInSourceDirectory << "\n"
                    << "to destination:\n"
                    << "    " << *destinationAbsolutePath
                    << "\n\n";
            TerminalAndLoggerSingleton::get().printAndLog(message.str());

            FileMoverSingleton::move(*pathToFileInSourceDirectory, *destinationAbsolutePath);
        }
    }
}

void MatchFinderForPackageFilesToLocallyInstalledPackages::moveEverythingFromDir(
        const AbsolutePath& absolutePathToSourceDirectoryAsText,
        const AbsolutePath& absolutePathToDestinationDirectoryAsText) const
{
    std::filesystem::path pathToSourceDirectory {absolutePathToSourceDirectoryAsText.getAbsolutePath()};

    try {
        std::filesystem::exists(pathToSourceDirectory);
    } catch (const std::filesystem::__cxx11::filesystem_error& exception) {
        std::stringstream message;
        message
                << exception.what() << "\n"
                << "Skipping scanning of directory: " << pathToSourceDirectory << "\n"
                << "Error: Insufficient permissions to access the files in the directory or file path doesn't exist." << "\n"
                << "Please, run this program with elevated priviledges as 'sudo' or 'root' user"
                   " and make sure the source file is present on the filesystem." << "\n"
                << "---\n";

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
        return;
    }

    for (const auto& fileInPikaurSystemCacheBuildDir : std::filesystem::directory_iterator(pathToSourceDirectory)) {
        auto pathToFileInSourceDirectory = std::make_unique<AbsolutePath>(fileInPikaurSystemCacheBuildDir.path().string());

        std::stringstream destinationAbsolutePathAsStream;
        destinationAbsolutePathAsStream
                << absolutePathToDestinationDirectoryAsText
                << fileInPikaurSystemCacheBuildDir.path().filename().string();
        auto destinationAbsolutePath = std::make_unique<AbsolutePath>(destinationAbsolutePathAsStream.str());

        std::stringstream message{};
        message << "Moving file or directory from source path:\n"
                << "    " << *pathToFileInSourceDirectory << "\n"
                << "to destination:\n"
                << "    " << *destinationAbsolutePath
                << "\n\n";
        TerminalAndLoggerSingleton::get().printAndLog(message.str());

        FileMoverSingleton::move(*pathToFileInSourceDirectory, *destinationAbsolutePath);
    }
}




















