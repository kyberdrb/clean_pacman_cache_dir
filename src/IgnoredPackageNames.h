//
// Created by laptop on 5/1/22.
//

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

    bool isPackageWithGivenNameIgnored(std::unique_ptr<IgnoredPackageName>& ignoredPackageNameCandidate) const;

    std::string generateReport() const;

private:
    std::vector<std::unique_ptr<IgnoredPackageName>> ignoredPackageNames;
};
