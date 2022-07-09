//
// Created by laptop on 7/9/22.
//

#include "TerminalSingleton.h"

#include <iostream>

const TerminalSingleton& TerminalSingleton::get() {
    // Source: https://stackoverflow.com/questions/50995599/make-unique-doesnt-compile-for-creating-a-singleton-instance/50995962#50995962
    // Create the instance either with or without parentheses;
    //  I prefer to write the parentheses to clearly show the constructor invokation
    static TerminalSingleton theOneAndOnlyTerminalSingletonInstance = TerminalSingleton();
    return theOneAndOnlyTerminalSingletonInstance;
}

const TerminalSingleton& TerminalSingleton::printText(const std::string& text) const {
    std::cout << text;
    return *this;
}

void TerminalSingleton::printText(const std::stringstream& textstream) const {
    std::cout << textstream.str();
}
