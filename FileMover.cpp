//
// Created by laptop on 5/29/22.
//

#include "FileMover.h"

#include <filesystem>
#include <iostream>

void FileMover::move(const AbsolutePath& from, const AbsolutePath& to) {
    // catch 'std::filesystem' exception to prevent memory leaks
    try {
        std::filesystem::rename(from.getAbsolutePath(), to.getAbsolutePath());
    } catch (const std::filesystem::__cxx11::filesystem_error& ex) {
        std::cout << ex.what() << "\n";

        std::cout << "Error: Insufficient permissions to move files or source file path doesn't exist." << "\n";
        std::cout << "Please, run this program with elevated priviledges as 'sudo' or 'root' user"
                "and make sure the source file is present on the filesystem.\n---\n";
    }
}
