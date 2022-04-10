//
// Created by laptop on 4/10/22.
//

#pragma once

#include <ostream>
#include <memory>
#include <set>
#include <string>

class Architectures {
public:
    void addArchitecture(std::string architecture);

    friend std::ostream& operator<<(std::ostream& out, const Architectures& architectures) {
        out << "\n";
        out << "===============================================\n\n";
        out << "LIST OF ARCHITECTURES\n\n";

        out << "Found " << architectures.architectures.size() << " architectures\n\n";

        for (const auto& architecture : architectures.architectures) {
            out << architecture << "\n";
        }

        return out;
    }

private:

    std::set<std::string> architectures;
};
