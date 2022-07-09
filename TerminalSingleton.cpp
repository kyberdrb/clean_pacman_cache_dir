//
// Created by laptop on 7/9/22.
//

#include "TerminalSingleton.h"

#include <iostream>

void TerminalSingleton::printText(const std::string& text) {
    std::cout << text;
}

void TerminalSingleton::printText(const std::stringstream& textstream) {
    std::cout << textstream.str();
}
