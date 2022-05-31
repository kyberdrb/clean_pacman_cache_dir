//
// Created by laptop on 5/30/22.
//

#pragma once

#include <memory>
#include <string>

class Filename {
public:
    explicit Filename(std::string nameOfFile);

    const std::string& getFilename() const;

    friend std::ostream& operator<<(std::ostream& out, const Filename& filename) {
        out << filename.nameOfFile;
        return out;
    }

    friend bool operator<(
            const std::unique_ptr<Filename>& packageFilename,
            const std::unique_ptr<Filename>& anotherPackageFilename)
    {
        return packageFilename->nameOfFile < anotherPackageFilename->nameOfFile;
    }

private:
    std::string nameOfFile;
};
