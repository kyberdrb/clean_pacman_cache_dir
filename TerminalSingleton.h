//
// Created by laptop on 7/25/22.
//

#pragma once

#include <memory>
#include <sstream>

class TerminalSingleton {
public:
    // Enables the 'std::make_unique' to access the private constructor of this class
    //  so that the instance of this class can be created with 'std::make_unique' only from within of this class
    //  and not from outside.
    //  I like this version from all the Singleton implementations the most, because of
    //    1. sufficiency/minimalism - only the 'get' function is accessible from outside - from the public static interface of the class
    //    2. humbleness/minimalism - only the class itself is needed - no derived classes i.e. no inheritance or polymorphism
    friend std::unique_ptr<TerminalSingleton> std::make_unique<TerminalSingleton>();
    static const TerminalSingleton& get();

    // Return reference to the instance to enable Fluent Interface, i.e. function chaining,
    //  to print text continuously
    const TerminalSingleton& printAndLog(const std::string& text) const;
    void printText(const std::stringstream& textStream) const;

    TerminalSingleton(const TerminalSingleton& otherTerminalSingleton) = delete;
    TerminalSingleton& operator=(const TerminalSingleton& otherTerminalSingleton) = delete;

    TerminalSingleton(TerminalSingleton&& otherTerminalSingleton) = delete;
    TerminalSingleton& operator=(TerminalSingleton&& otherTerminalSingleton) = delete;

private:
    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance;
    std::stringstream logFilePathAsStream;

    TerminalSingleton();
};
