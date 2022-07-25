//
// Created by laptop on 7/25/22.
//

#include "TerminalSingleton.h"

#include <iostream>

const TerminalSingleton& TerminalSingleton::get() {
    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance;
    return *theOneAndOnlyTerminalSingletonInstance;
}

const TerminalSingleton& TerminalSingleton::printText(const std::string& text) const {
    std::cout << text;
    return *this;
}

void TerminalSingleton::printText(const std::stringstream& textstream) const {
    std::cout << textstream.str();
}
