//
// Created by laptop on 7/19/22.
//

#pragma once

#include <memory>
#include <sstream>

class TerminalSingleton {
public:
    static const TerminalSingleton& get();

    // Return reference to the instance to enable Fluent Interface, i.e. function chaining,
    //  to print text continuously
    const TerminalSingleton& printText(const std::string& text) const;
    void printText(const std::stringstream& textStream) const;

    TerminalSingleton(const TerminalSingleton& otherTerminalSingleton) = delete;
    TerminalSingleton& operator=(const TerminalSingleton& otherTerminalSingleton) = delete;

    TerminalSingleton(TerminalSingleton&& otherTerminalSingleton) = delete;
    TerminalSingleton& operator=(TerminalSingleton&& otherTerminalSingleton) = delete;

private:
    TerminalSingleton() = default;
};
