//
// Created by laptop on 6/1/22.
//

#pragma once

#include "Package__refactored__.h"

#include <iostream>

class LocallyInstalledPackage__refactored__ : public Package__refactored__ {
public:
    void commonFunction() override {
        std::cout << "Common function - from LocallyInstalledPackage__refactored__" << "\n";
    }

    void functionOnlyInLocallyInstalledPackage__refactored__() {
        std::cout << "Function only in LocallyInstalledPackage__refactored__" << "\n";
    }
};
