//
// Created by laptop on 7/5/22.
//

#include "TerminalSingleton.h"

#include <iostream>

const TerminalSingleton& TerminalSingleton::get() {
    struct TerminalSingletonDerived : public TerminalSingleton{};

    static std::unique_ptr<TerminalSingletonDerived> theOneAndOnlyTerminalSingletonDerivedInstance =
            std::make_unique<TerminalSingletonDerived>();

    return *theOneAndOnlyTerminalSingletonDerivedInstance;
}

const TerminalSingleton& TerminalSingleton::printText(std::string text) const {
    std::cout << text;
    return *this;
}

//void TerminalSingleton::printNewLine() const {
//    TerminalSingleton::printText("\n");
//}
