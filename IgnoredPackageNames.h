//
// Created by laptop on 6/17/22.
//

#pragma once

#include "IgnoredPackageName.h"

#include <memory>
#include <vector>

class IgnoredPackageNames {
public:
    IgnoredPackageNames();

    void emplace_back(std::unique_ptr<IgnoredPackageName> ignoredPackageName);

    bool isPackageWithGivenNameIgnored(std::unique_ptr<IgnoredPackageName>& ignoredPackageNameCandidate) const;

    std::string generateReport() const;

private:
    std::vector<std::unique_ptr<IgnoredPackageName>> ignoredPackageNames;
};
