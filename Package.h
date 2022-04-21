#pragma once

#include <string>
#include <vector>
#include <ostream>

class Package {
public:
    Package(std::string name, std::string locallyInstalledVersion, std::string architecture);

    // TODO maybe delete this function altogether
    std::vector<std::string> getPackageNameCandidates() const;

    std::string getLocallyInstalledVersion() const;

    bool isSpecial() const;

    std::string buildPartialPackageNamePrefix() const;

    bool isEmpty() const;

    friend std::ostream &operator<<(std::ostream &out, const Package &package) {
        out << package.name << "\t" << package.locallyInstalledVersion << "\t" << package.architecture << "\t" << package.name << "-" << package.locallyInstalledVersion << "-" << package.architecture;

        if ( ! package.name.empty() && std::isdigit(package.name.at(0) ) ) {
            out << "\t" << "PACKAGE NAME BEGINNS WITH A NUMBER";
        }

        if ( ! package.locallyInstalledVersion.empty() && ! std::isdigit(package.locallyInstalledVersion.at(0) ) ) {
            out << "\t" << "PACKAGE VERSION BEGINNS WITH A LETTER";
        }

        return out;
    }

    const std::string &getName() const;

    bool operator==(const Package &rhs) const {
        return name == rhs.name &&
               locallyInstalledVersion == rhs.locallyInstalledVersion &&
               architecture == rhs.architecture;
    }

private:
    std::string name;
    std::string locallyInstalledVersion;
    std::string architecture;
};
