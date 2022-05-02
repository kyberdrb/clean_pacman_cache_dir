//
// Created by laptop on 5/1/22.
//

#pragma once

#include "PackageName.h"

#include <memory>
#include <vector>

class IgnoredPackageNames {
public:
    IgnoredPackageNames();
    explicit IgnoredPackageNames(std::string pacmanConfigurationFilePath);

    bool contains(const PackageName& packageName) const;

    std::string generateReport() const;

private:
    std::vector<std::unique_ptr<PackageName>> ignoredPackageNames;
    std::string pacmanConfigurationFilePath;

    void setDefaultPacmanConfigPathIfEmpty();
    void findIgnoredPackageNames();
    
    friend std::ostream& operator<<(std::ostream& out, const IgnoredPackageNames& ignoredPackageNamesInstance) {
        out << "\n";
        out << "===============================================\n\n";
        out << "LIST OF IGNORED PACKAGES\n\n";

        out << "Found " << ignoredPackageNamesInstance.ignoredPackageNames.size() << " ignored packages\n\n";

        for (const auto& ignoredPackageName : ignoredPackageNamesInstance.ignoredPackageNames) {
            out << *ignoredPackageName  << "\n";
        }

        return out;
    }
};
