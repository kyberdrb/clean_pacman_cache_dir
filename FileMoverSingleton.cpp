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
        // TODO What happens when the source file/directory is renamed/moved to destination
        //  which already contains a file/directory with the same name?
        //  - Will the 'rename' handle this conflict situation by itself? How?
        //    - By throwing an exception?
        //    - By overwriting without prompting?
        //  - Do I manually append a number to the filename? From '1' to whichever is available next,
        //    when there are multiple duplicate files/dirs with the same name?
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
