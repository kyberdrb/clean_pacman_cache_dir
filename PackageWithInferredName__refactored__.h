//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package__refactored__.h"

#include <iostream>

class PackageWithInferredName__refactored__ : public Package__refactored__ {
public:
    void commonFunction() override {
        std::cout << "Common function - from PackageWithInferredName__refactored__" << "\n";
    }

    void functionOnlyInPackageWithInferredName__refactored__() {
        std::cout << "Function only in PackageWithInferredName__refactored__" << "\n";
    }
};
