//
// Created by laptop on 9/5/22.
//

#pragma once

#include <memory>
#include <sstream>

class TerminalSingleton {
public:
    static void printText(const std::string& text);
    static void printText(const std::stringstream& textstream);

    // Disable copy semantics:
    //   - disable copy constructor
    TerminalSingleton(const TerminalSingleton& otherTerminalSingleton) = delete;

    //   - disable copy assignment operator
    TerminalSingleton& operator=(const TerminalSingleton& otherTerminalSingleton) = delete;

    // Disable move semantics
    //   - disable move constructor
    TerminalSingleton(TerminalSingleton&& otherTerminalSingleton) = delete;

    //   - disable move assignment operator
    TerminalSingleton& operator=(TerminalSingleton&& otherTerminalSingleton) = delete;

private:
    TerminalSingleton() = default;
};
