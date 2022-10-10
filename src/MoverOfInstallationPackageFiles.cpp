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

    // catch 'std::filesystem' exception to prevent memory leaks when the user doesn't have the priviledges to create directory
    try {
        std::filesystem::create_directories(Paths::get().getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText());

        const auto directoryForInstallationPackageFilesForDeletion =
                std::make_unique<AbsolutePath>(Paths::get().getPathToDirectoryForInstallationPackageFilesDeletionCandidatesAsText());

        if (std::filesystem::exists(directoryForInstallationPackageFilesForDeletion->getAbsolutePath())) {
            this->locallyInstalledPackages.movePackageFilesForDifferentPackageVersionsToSeparateDir(*directoryForInstallationPackageFilesForDeletion);
            this->packageFilesRelatedToPackages.moveChosenInstallationPackageFiles(*directoryForInstallationPackageFilesForDeletion);
            this->packageFilesRelatedToPackages.cleanUpOtherFilesInPikaurCacheDirs(*directoryForInstallationPackageFilesForDeletion);
        }
    } catch (const std::filesystem::__cxx11::filesystem_error& exception) {
        message
                << exception.what() << "\n"
                << "Error: Insufficient permissions to create directory for package related files for deletion." << "\n"
                << "Please, run this program with elevated priviledges as 'sudo' or 'root' user." << "\n"
                << dividingDashedLine;

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
    }
}
