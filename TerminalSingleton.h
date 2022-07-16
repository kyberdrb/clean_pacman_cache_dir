//
// Created by laptop on 7/16/22.
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
    void printText(const std::stringstream& textstream) const;

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
    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance;

    TerminalSingleton() = default;
};
