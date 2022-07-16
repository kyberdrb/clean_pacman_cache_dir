//
// Created by laptop on 7/16/22.
//

#include "TerminalSingleton.h"

#include <iostream>

// Derived class 'TerminalSingletonDerived' can be instantiated and destroyed entirely,
//  even though it's in an inheritance relationship with 'TerminalSingleton'
//  because the body of the derived class is emtpy,
//  i.e. the class has no member functions or variables, which means that no object-slicing
//  or partial destruction occurs; nice, safe and convenient trick/solution
class TerminalSingletonDerived : public TerminalSingleton {};
std::unique_ptr<TerminalSingleton> TerminalSingleton::theOneAndOnlyTerminalSingletonInstance = std::make_unique<TerminalSingletonDerived>();

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
