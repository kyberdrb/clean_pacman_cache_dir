//
// Created by laptop on 7/9/22.
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
    // Define only the default constructor and make it private
    //  so that only the Singleton class controls the creation and access to the single instance
    TerminalSingleton() = default;
};
