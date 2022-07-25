//
// Created by laptop on 7/25/22.
//

#include "TerminalSingleton.h"

#include <iostream>

// Formal/minimal constructor implementation
TerminalSingleton::TerminalSingleton(const PrivateAuthorizationToken& privateAuthorizationToken) {}

const TerminalSingleton& TerminalSingleton::get() {
    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance =
            std::make_unique<TerminalSingleton>(PrivateAuthorizationToken());

    return *theOneAndOnlyTerminalSingletonInstance;
}

const TerminalSingleton& TerminalSingleton::printText(const std::string& text) const {
    std::cout << text;
    return *this;
}

void TerminalSingleton::printText(const std::stringstream& textstream) const {
    std::cout << textstream.str();
}
