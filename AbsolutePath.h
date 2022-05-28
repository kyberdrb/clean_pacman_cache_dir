//
// Created by laptop on 5/28/22.
//

#pragma once

#include <string>

class AbsolutePath {
public:
    explicit AbsolutePath(std::string absolutePath);

    const std::string& getAbsolutePath() const;

    friend std::ostream& operator<<(std::ostream& out, const AbsolutePath& absolutePath) {
        out << absolutePath.path;
        return out;
    }

    std::string operator+(const std::string& suffix) const{
        return this->path + suffix;
    }

private:
    std::string path;
};
