#pragma once

#include <string>
#include <vector>
#include <ostream>

class Package {
public:
    Package(std::string name, std::string locallyInstalledVersion, std::string architecture);

    std::vector<std::string> getPackageNameCandidates() const;

    std::string getLocallyInstalledVersion() const;

    friend std::ostream &operator<<(std::ostream &os, const Package &package) {
        os << package.name << "\t" << package.locallyInstalledVersion << "\t" << package.architecture;
        return os;
    }

private:
    std::string name;
    std::string locallyInstalledVersion;
    std::string architecture;
};
