//
// Created by laptop on 5/28/22.
//

#pragma once

#include "Filename.h"

class AbsolutePath {
public:
    explicit AbsolutePath(std::string absolutePath);

    const std::string& getAbsolutePath() const;

    friend std::ostream& operator<<(std::ostream& out, const AbsolutePath& absolutePath) {
        out << absolutePath.path;
        return out;
    }

    // TODO return a unique_ptr to AbsolutePath instead of a string?
    std::string operator+(const Filename& filename) const{
        return this->path + filename.getFilename();
    }

private:
    std::string path;
};
