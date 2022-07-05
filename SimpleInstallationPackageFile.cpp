//
// Created by laptop on 5/26/22.
//

#include "SimpleInstallationPackageFile.h"

#include "FileMover.h"

#include <iostream>

SimpleInstallationPackageFile::SimpleInstallationPackageFile(
        std::unique_ptr<AbsolutePath> absolutePath,
        std::unique_ptr<Filename> filename,
        SimpleInstallationPackageFileType reasonForDeletion)
:
        absolutePath(std::move(absolutePath)),
        filename(std::move(filename)),
        reasonForDeletion(reasonForDeletion)
{}

const AbsolutePath& SimpleInstallationPackageFile::getAbsolutePath() const {
    return *(this->absolutePath);
}

const Filename& SimpleInstallationPackageFile::getFilename() const {
    return *(this->filename);
}

void SimpleInstallationPackageFile::moveToSeparateDirectoryForDeletion(const AbsolutePath& directoryForInstallationPackageFilesForDeletion) {
    const AbsolutePath& from = this->getAbsolutePath();
    const auto to = directoryForInstallationPackageFilesForDeletion + this->getFilename();

    if (this->reasonForDeletion == SimpleInstallationPackageFileType::PARTIALLY_DOWNLOADED) {
        std::cout
            << "Moving partially downloaded installation package file:\n"
            << "\t" << from << "\n"
            << "to separate directory\n"
            << "\t" << *(to) << "\n\n";
    } else if (this->reasonForDeletion == SimpleInstallationPackageFileType::MISSING_LOCALLY_INSTALLED_PACKAGE) {
        std::cout
            << "Moving installation package file related to missing package:\n"
            << "\t" << from << "\n"
            << "to separate directory\n"
            << "\t" << *(to) << "\n\n";
    }

    FileMover::move(from, *(to));
}
