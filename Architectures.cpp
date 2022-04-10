//
// Created by laptop on 4/10/22.
//

#include "Architectures.h"

void Architectures::addArchitecture(std::string architecture) {
    this->architectures.emplace(std::move(architecture));
}
