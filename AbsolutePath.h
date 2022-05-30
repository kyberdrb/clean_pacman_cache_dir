//
// Created by laptop on 5/28/22.
//

#pragma once

#include "Filename.h"

#include <memory>

class AbsolutePath {
public:
    explicit AbsolutePath(std::string absolutePath);

    const std::string& getAbsolutePath() const;

    friend std::ostream& operator<<(std::ostream& out, const AbsolutePath& absolutePath) {
        out << absolutePath.path;
        return out;
    }

    std::unique_ptr<AbsolutePath> operator+(const Filename& filename) const {
        return std::make_unique<AbsolutePath>(this->path + filename.getFilename());
    }

private:
    std::string path;
};
