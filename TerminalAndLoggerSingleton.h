//
// Created by laptop on 7/25/22.
//

#pragma once

#include <memory>
#include <sstream>

class TerminalAndLoggerSingleton {
public:
    // Enables the 'std::make_unique' to access the private constructor of this class
    //  so that the instance of this class can be created with 'std::make_unique' only from within of this class
    //  and not from outside.
    //  I like this version from all the Singleton implementations the most, because of
    //    1. sufficiency/minimalism - only the 'get' function is accessible from outside - from the public static interface of the class
    //    2. humbleness/minimalism - only the class itself is needed - no derived classes i.e. no inheritance or polymorphism
    friend std::unique_ptr<TerminalAndLoggerSingleton> std::make_unique<TerminalAndLoggerSingleton>();
    static const TerminalAndLoggerSingleton& get();

    // Return reference to the instance to enable Fluent Interface, i.e. function chaining,
    //  to print text continuously
    const TerminalAndLoggerSingleton& printAndLog(const std::string& text) const;
    void printTextWithoutLogging(const std::stringstream& textStream) const;

    const std::string& getLogFilePath() const;

    TerminalAndLoggerSingleton(const TerminalAndLoggerSingleton& otherTerminalAndLoggerSingleton) = delete;
    TerminalAndLoggerSingleton& operator=(const TerminalAndLoggerSingleton& otherTerminalAndLoggerSingleton) = delete;

    TerminalAndLoggerSingleton(TerminalAndLoggerSingleton&& otherTerminalAndLoggerSingleton) = delete;
    TerminalAndLoggerSingleton& operator=(TerminalAndLoggerSingleton&& otherTerminalAndLoggerSingleton) = delete;

private:
    static std::unique_ptr<TerminalAndLoggerSingleton> theOneAndOnlyTerminalAndLoggerSingletonInstance;
    std::string logFilePath;

    TerminalAndLoggerSingleton();

    std::string determineLogFilePath();
};
