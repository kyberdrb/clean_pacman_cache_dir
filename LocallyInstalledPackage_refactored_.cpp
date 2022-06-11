//
// Created by laptop on 6/8/22.
//

//
// Created by laptop on 4/9/22.
//

#include "LocallyInstalledPackage_refactored_.h"

#include "FileMover.h"

#include <iostream>

LocallyInstalledPackage_refactored_::LocallyInstalledPackage_refactored_(
    std::unique_ptr<PackageName> packageName,
    std::unique_ptr<PackageVersion> locallyInstalledVersion,
    std::string architecture,
    bool isIgnored)
:
    Package_refactored_(std::move(packageName)),
    locallyInstalledVersion(std::move(locallyInstalledVersion)),
    architecture(std::move(architecture)),
    isIgnored(isIgnored)
{}

// TODO remove after delegation to the base class 'Package_refactored_'
//const PackageName& LocallyInstalledPackage_refactored_::getName() const {
//    return *(this->name);
//}

uint_fast16_t LocallyInstalledPackage_refactored_::getNumberOfInstallationPackageFilesForDifferentVersions() const {
    return this->installationPackageFilesForDifferentPackageVersions.size();
}

bool LocallyInstalledPackage_refactored_::addPackageFileToDeletionCandidates(
        std::unique_ptr<ExtendedInstallationPackageFile> packageRelatedPackageFile)
{
    // TODO remove after delegation to the base class 'Package_refactored_'
//    bool isPackageNamesMatching = *(this->name) == packageRelatedPackageFile->getRelatedPackageName();

    bool isPackageNamesMatching = Package_refactored_::getName() == packageRelatedPackageFile->getRelatedPackageName();
    bool isPackageVersionDifferent = *(this->locallyInstalledVersion) != packageRelatedPackageFile->getRelatedPackageVersion();

    // For debugging purposes
//    if (isPackageVersionDifferent) {
//        std::cerr << "this is what I was waiting for..." << "\n";
//    }

    bool isPackageNonignored = !this->isIgnored;

    if ( isPackageNamesMatching && isPackageVersionDifferent && isPackageNonignored) {
        this->installationPackageFilesForDifferentPackageVersions.emplace_back(std::move(packageRelatedPackageFile));
        return true;
    }

    return false;
}

void LocallyInstalledPackage_refactored_::movePackageFilesForDifferentVersionsToSeparateDir(
        const AbsolutePath& absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles)
{
    for (const auto& packageFileForDeletion : this->installationPackageFilesForDifferentPackageVersions) {
        const AbsolutePath& from = packageFileForDeletion->getAbsolutePath();
        const auto to = absolutePathToDirectoryForOtherVersionsOfInstallationPackageFiles + packageFileForDeletion->getFilename();

        std::cout
            << "Info about the locally installed package:\n"

            // TODO remove after delegation to the base class 'Package_refactored_'
//            << "\t" << *(this->name) << "-" << *(this->locallyInstalledVersion) << "\n";

            << "\t" << Package_refactored_::getName() << "-" << *(this->locallyInstalledVersion) << "\n";

        std::cout
            << "Info about the installation package file for deletion:\n"
            << "\t" << packageFileForDeletion->getRelatedPackageName() << "-" << packageFileForDeletion->getRelatedPackageVersion() << "\n";

        std::cout << "\n";

        // TODO maybe abstract duplicate code from here and from 'moveToSeparateDirectoryForDeletion' in 'SimpleInstallationPackageFile.cpp'
        //  to a separate class 'TerminalPrinter'? for example?
        //TerminalPrinter::printInstallationPackageFileInfoBeforeDeletion(from, *(to));
        std::cout
            << "Moving package file:\n"
            << "\t" << from << "\n"
            << "to separate directory:\n"
            << "\t" << *(to) << "\n\n";

        FileMover::move(from, *(to));
    }
}
