//
// Created by laptop on 5/28/22.
//

#include "AbsolutePath.h"

AbsolutePath::AbsolutePath(std::string absolutePath) :
        path(std::move(absolutePath))
{}

const std::string& AbsolutePath::getAbsolutePath() const {
    return AbsolutePath::path;
}
