//
// Created by laptop on 7/25/22.
//

#include "TerminalAndLoggerSingleton.h"

#include "Paths.h"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>

// Making the 'std::make_unique' a friend of this class fixes the error message in clang-tidy:
//     "calling a private constructor of class 'TerminalAndLoggerSingleton'"
//  and its related error message from the compiler
//     "error: ‘constexpr TerminalAndLoggerSingleton::TerminalAndLoggerSingleton()’ is private within this context"
std::unique_ptr<TerminalAndLoggerSingleton> TerminalAndLoggerSingleton::theOneAndOnlyTerminalAndLoggerSingletonInstance =
        std::make_unique<TerminalAndLoggerSingleton>();

TerminalAndLoggerSingleton::TerminalAndLoggerSingleton() :
        logFilePath(this->determineLogFilePath())
{}

std::string TerminalAndLoggerSingleton::determineLogFilePath() {
    std::filesystem::create_directories(Paths::get().getLogFileDir());

    std::stringstream logFileNameAsStream{};
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    logFileNameAsStream << std::put_time(localtime(&in_time_t), "%Y_%m_%d-%H_%M_%S"); // formatting string inspired by '$ man date'
    logFileNameAsStream << ".log";

    std::stringstream logFilePathAsStream{};
    logFilePathAsStream << Paths::get().getLogFileDir() << logFileNameAsStream.str();
    return logFilePathAsStream.str();
}

const TerminalAndLoggerSingleton& TerminalAndLoggerSingleton::get() {
    return *(TerminalAndLoggerSingleton::theOneAndOnlyTerminalAndLoggerSingletonInstance);
}

const TerminalAndLoggerSingleton& TerminalAndLoggerSingleton::printAndLog(const std::string& text) const {
    std::cout << text;

    std::ofstream logFile{logFilePath, std::ios::app};
    logFile << text;

    return *this;
}

void TerminalAndLoggerSingleton::printTextWithoutLogging(const std::stringstream& textstream) const {
    std::cout << textstream.str();
}

const std::string& TerminalAndLoggerSingleton::getLogFilePath() const {
    return this->logFilePath;
}