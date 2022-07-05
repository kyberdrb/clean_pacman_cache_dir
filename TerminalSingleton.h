//
// Created by laptop on 7/5/22.
//

#pragma once

#include <memory>

class TerminalSingleton {

public:
    static const TerminalSingleton& get();

    // Return reference to the instance to enable Fluent Interface, i.e. function chaining
    const TerminalSingleton& printText(std::string text) const;

//    void printNewLine() const;

private:
    std::unique_ptr<TerminalSingleton> terminalSingleton;

    TerminalSingleton() = default;
};
