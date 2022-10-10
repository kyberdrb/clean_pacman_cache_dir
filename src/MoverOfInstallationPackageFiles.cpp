//
// Created by laptop on 6/20/22.
//

#include "MoverOfInstallationPackageFiles.h"

#include "Paths.h"
#include "TerminalAndLoggerSingleton.h"

#include <filesystem>

MoverOfInstallationPackageFiles::MoverOfInstallationPackageFiles(
        const MatchFinderForPackageFilesToLocallyInstalledPackages& packageFilesRelatedToLocallyInstalledPackages,
        const LocallyInstalledPackages& locallyInstalledPackages)
:
        packageFilesRelatedToPackages(packageFilesRelatedToLocallyInstalledPackages),
        locallyInstalledPackages(locallyInstalledPackages)
{}

void MoverOfInstallationPackageFiles::moveChosenInstallationPackageFilesToSeparateDir() const {
    std::stringstream message;

    message
        << "\n"
        << "===============================================\n\n"
        << "MOVING PACKAGES\n\n";

    TerminalAndLoggerSingleton::get().printAndLog(message.str());

    message.str("");
    message.clear();

    std::string dividingDashedLine = "---\n";

    // TODO this try-catch code catches the same exception as in "FileMover" class:
    //   merge/centralize/delegate filesystem operations and filesystem exception handling
    //   to a separate class "File" or "Filesystem"?
    // catch 'std::filesystem' exception to prevent memory leaks when the user doesn't have the priviledges to create directory
    try {
        std::filesystem::create_directories(Paths::get().getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText());
    } catch (const std::filesystem::__cxx11::filesystem_error& exception) {
        message
                << exception.what() << "\n"
                << "Error: Insufficient permissions to create directory for package related files for deletion." << "\n"
                << "Please, run this program with elevated priviledges as 'sudo' or 'root' user." << "\n"
                << "Skipping moving files..." << "\n"
                << "\n"
                << dividingDashedLine;

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
    }

    const auto directoryForInstallationPackageFilesForDeletion =
            std::make_unique<AbsolutePath>(Paths::get().getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText());

    message.str("");
    message.clear();

    bool doesDirForPackageRelatedFilesForDeletionExist =
            std::filesystem::exists(directoryForInstallationPackageFilesForDeletion->getAbsolutePath());

    if (doesDirForPackageRelatedFilesForDeletionExist) {
        this->locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
        this->packageFilesRelatedToPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
        this->packageFilesRelatedToPackages.cleanUpOtherFilesInPikaurCacheDirs(*directoryForInstallationPackageFilesForDeletion);

        message << "\n"
                << dividingDashedLine
                << "\n"
                << "The package related files for deletion are stored at:"
                << "\n"
                << *directoryForInstallationPackageFilesForDeletion << "\n"
                << "\n"
                << dividingDashedLine;

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
    }
}
