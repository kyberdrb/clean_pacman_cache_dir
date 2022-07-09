//
// Created by laptop on 6/8/22.
//

//
// Created by laptop on 4/9/22.
//

#include "LocallyInstalledPackage.h"

#include "FileMoverSingleton.h"
#include "TerminalSingleton.h"

#include <sstream>

// For debugging purposes
//#include "TerminalSingleton.h"

LocallyInstalledPackage::LocallyInstalledPackage(
    std::unique_ptr<PackageName> packageName,
    std::unique_ptr<PackageVersion> locallyInstalledVersion,
    std::string architecture,
    bool isIgnored)
:
        Package(std::move(packageName)),
        locallyInstalledVersion(std::move(locallyInstalledVersion)),
        architecture(std::move(architecture)),
        isIgnored(isIgnored)
{}

uint_fast16_t LocallyInstalledPackage::getNumberOfInstallationPackageFilesForDifferentVersions() const {
    return this->installationPackageFilesForDifferentPackageVersions.size();
}

bool LocallyInstalledPackage::addPackageFileToDeletionCandidates(
        std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile)
{
    bool isPackageNamesMatching = Package::getName() == packageRelatedPackageFile->getRelatedPackageName();
    bool isPackageVersionDifferent = *(this->locallyInstalledVersion) != packageRelatedPackageFile->getRelatedPackageVersion();

    // For debugging purposes
//    if (isPackageVersionDifferent) {
//        TerminalSingleton::get().printText("this is what I was waiting for...\n");
//    }

    bool isPackageNonignored = !this->isIgnored;

    if ( isPackageNamesMatching && isPackageVersionDifferent && isPackageNonignored) {
        this->installationPackageFilesForDifferentPackageVersions.emplace_back(std::move(packageRelatedPackageFile));
        return true;
    }

    return false;
}

void LocallyInstalledPackage::movePackageFilesForDifferentVersionsToSeparateDir(
        const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles) const
{
    for (const auto& packageFileForDeletion : this->installationPackageFilesForDifferentPackageVersions) {
        std::stringstream message;
        const AbsolutePath& from = packageFileForDeletion->getAbsolutePath();
        const auto to = absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles + packageFileForDeletion->getFilename();

        message
                << "Info about the locally installed package:\n"
                << "\t" << Package::getName() << "-" << *(this->locallyInstalledVersion) << "\n"
                << "Info about the installation package file for deletion:\n"
                << "\t" << packageFileForDeletion->getRelatedPackageName() << "-" << packageFileForDeletion->getRelatedPackageVersion() << "\n"
                << "\n"
                << "Moving package file:\n"
                << "\t" << from << "\n"
                << "to separate directory:\n"
                << "\t" << *(to) << "\n\n";

        TerminalSingleton::get().printText(message);
        FileMoverSingleton::move(from, *(to));
    }
}
