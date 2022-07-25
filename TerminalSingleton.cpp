//
// Created by laptop on 7/25/22.
//

#include "TerminalSingleton.h"

#include <iostream>

// Making the 'std::make_unique' a friend of this class fixes the error message in clang-tidy:
//     "calling a private constructor of class 'TerminalSingleton'"
//  and its related error message from the compiler
//     "error: ‘constexpr TerminalSingleton::TerminalSingleton()’ is private within this context"
std::unique_ptr<TerminalSingleton> TerminalSingleton::theOneAndOnlyTerminalSingletonInstance = std::make_unique<TerminalSingleton>();

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
