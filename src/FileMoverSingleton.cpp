//
// Created by laptop on 5/29/22.
//

#include "FileMoverSingleton.h"

#include "TerminalAndLoggerSingleton.h"

#include <filesystem>
#include <sstream>

void FileMoverSingleton::move(const AbsolutePath& from, const AbsolutePath& to) {
    std::stringstream message{};
    std::string dividingDashedLine = "---\n";

    // catch 'std::filesystem' exception to prevent memory leaks when 'rename' fails
    try {
        // Handle possible duplicate filename/dirname conflicts in a preserving way,
        //  i.e. when moving a file to destination and the destination dir already has file(s) with the same name, keep all of them
        //  by distinguishing the duplicates in their filename
        bool doesDestinationPathExist = std::filesystem::exists(to.getAbsolutePath());
        if (doesDestinationPathExist) {
            for (int_fast32_t duplicateNumber = 1; duplicateNumber < INT_FAST32_MAX; ++duplicateNumber) {
                std::stringstream nameOfDuplicateFile;

                nameOfDuplicateFile << to.getAbsolutePath() << "-" << duplicateNumber;

                const auto& nameOfDuplicateFileAsText = nameOfDuplicateFile.str();
                bool nameOfDuplicateFileIsAvailable = !std::filesystem::exists(nameOfDuplicateFileAsText);
                if (nameOfDuplicateFileIsAvailable) {
                    std::filesystem::rename(from.getAbsolutePath(), nameOfDuplicateFileAsText);
                    return;
                }

                nameOfDuplicateFile.str(std::string{});
            }
        }

        std::filesystem::rename(from.getAbsolutePath(), to.getAbsolutePath());
        TerminalAndLoggerSingleton::get().printAndLog(dividingDashedLine);
    } catch (const std::filesystem::__cxx11::filesystem_error& exception) {
        message
            << exception.what() << "\n"
            << "Error: Insufficient permissions to move files or source file path doesn't exist." << "\n"
            << "Please, run this program with elevated priviledges as 'sudo' or 'root' user"
               " and make sure the source file is present on the filesystem." << "\n"
           << dividingDashedLine;

        TerminalAndLoggerSingleton::get().printAndLog(message.str());
    }
}
