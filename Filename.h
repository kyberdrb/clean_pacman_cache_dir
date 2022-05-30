//
// Created by laptop on 5/30/22.
//

#pragma once

#include <string>

class Filename {
public:
    explicit Filename(std::string nameOfFile);

    const std::string& getFilename() const;

    friend std::ostream& operator<<(std::ostream& out, const Filename& filename) {
        out << filename.nameOfFile;
        return out;
    }

private:
    std::string nameOfFile;
};
