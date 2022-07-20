//
// Created by laptop on 7/20/22.
//

#include "TerminalSingleton.h"

#include <iostream>

std::unique_ptr<TerminalSingleton> TerminalSingleton::theOneAndOnlyTerminalSingletonInstance;

const TerminalSingleton& TerminalSingleton::get() {
    return *(TerminalSingleton::theOneAndOnlyTerminalSingletonInstance);
}

const TerminalSingleton& TerminalSingleton::printText(const std::string& text) const {
    std::cout << text;
    return *this;
}

void TerminalSingleton::printText(const std::stringstream& textstream) const {
    std::cout << textstream.str();
}
