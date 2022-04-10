#pragma once

#include <string>
#include <vector>

class Package {
public:
    Package(std::string name, std::string version, std::string architecture);

    std::vector<std::string> getPackageNameCandidates() const;

private:
    std::string name;
    std::string installedVersion;
    std::string architecture;
};
