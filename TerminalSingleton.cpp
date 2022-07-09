//
// Created by laptop on 7/9/22.
//

#include "TerminalSingleton.h"

#include <iostream>

// Create the instance either with or without parentheses;
//  I prefer to write the constructor invokation explicitly to clearly show the instance creation
TerminalSingleton TerminalSingleton::theOneAndOnlyTerminalSingletonInstance = TerminalSingleton();

const TerminalSingleton& TerminalSingleton::get() {
    return TerminalSingleton::theOneAndOnlyTerminalSingletonInstance;
}

const TerminalSingleton& TerminalSingleton::printText(const std::string& text) const {
    std::cout << text;
    return *this;
}

void TerminalSingleton::printText(const std::stringstream& textstream) const {
    std::cout << textstream.str();
}
