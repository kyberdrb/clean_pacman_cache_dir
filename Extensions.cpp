//
// Created by laptop on 4/9/22.
//

#include "Extensions.h"

void Extensions::addExtension(std::string extension) {
    this->extensions.emplace(std::move(extension));
}
