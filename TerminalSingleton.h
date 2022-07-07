//
// Created by laptop on 7/5/22.
//

#pragma once

#include <memory>

class TerminalSingleton {
public:
    static const TerminalSingleton& get();

    // Enables the 'std::make_unique' to access the private constructor of this class
    //  so that the instance of this class can be created with 'std::make_unique' only from within of this class
    //  and not from outside.
    //  I like this version of the Singleton implementation better, because
    //    1. sufficiency - only the 'get' function is accessible from outside
    //    2. humbleness - only the class itself is needed - no derived classes i.e. no inheritance or polymorphism
//    friend std::unique_ptr<TerminalSingleton> std::make_unique<TerminalSingleton>();

    // Return reference to the instance to enable Fluent Interface, i.e. function chaining,
    //  to print text continuously
    const TerminalSingleton& printText(const std::string& text) const;

    TerminalSingleton(const TerminalSingleton& otherTerminalSingleton) = delete;
    TerminalSingleton& operator=(const TerminalSingleton& otherTerminalSingleton) = delete;

private:
//    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance;

    TerminalSingleton() = default;
};
