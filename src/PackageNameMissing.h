//
// Created by laptop on 6/18/22.
//

#pragma once

#include "Package.h"

#include <exception>
#include <sstream>

class PackageNameMissing : public std::exception {
public:
    explicit PackageNameMissing(const Package& packageWithInferredName);

    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return this->exceptionMessageAsCharSequence.c_str();
    }

private:
    const PackageName& inferredPackageName;
    std::string exceptionMessageAsCharSequence;
};
