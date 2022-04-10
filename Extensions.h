//
// Created by laptop on 4/9/22.
//

#pragma once

#include <ostream>
#include <memory>
#include <set>
#include <string>

class Extensions {
public:
    void addExtension(std::string extension);
    
    friend std::ostream& operator<<(std::ostream& out, const Extensions& extensions) {
        out << "\n";
        out << "===============================================\n\n";
        out << "LIST OF EXTENSIONS\n\n";

        out << "Found " << extensions.extensions.size() << " extensions\n\n";

        for (const auto& extension : extensions.extensions) {
            out << extension << "\n";
        }

        return out;
    }

private:

    std::set<std::string> extensions;
};
