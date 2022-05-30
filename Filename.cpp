//
// Created by laptop on 5/30/22.
//

#include "Filename.h"

Filename::Filename(std::string filename) :
    nameOfFile(std::move(filename))
{}

const std::string& Filename::getFilename() const {
    return this->nameOfFile;
}