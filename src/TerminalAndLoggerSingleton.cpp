//
// Created by laptop on 7/25/22.
//

#include "TerminalAndLoggerSingleton.h"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

// TODO centralize duplicate code by encapsulating the home dir detection to a separate class
//  and use it here and in 'MatchFinderForPackageFilesToLocallyInstalledPackages.cpp'
// helpers to detect the home directory path of the current user, even behind 'sudo'
#include <libaudit.h> // for 'audit_getloginuid()' to detect the UID of the user who invoked 'sudo', instead of the 'root' user
#include <pwd.h> // for 'getpwuid()' to get the home directory for the given UID

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
    std::stringstream logFileDirPathAsStream{};
    std::string currentUserHomeDir = getpwuid(audit_getloginuid())->pw_dir;
    logFileDirPathAsStream << currentUserHomeDir;
    logFileDirPathAsStream << "/.config/cpmcd/logs/";
    std::filesystem::create_directories(logFileDirPathAsStream.str());

    std::stringstream logFileNameAsStream{};
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    logFileNameAsStream << std::put_time(localtime(&in_time_t), "%Y_%m_%d-%H_%M_%S"); // formatting string inspired by '$ man date'
    logFileNameAsStream << ".log";

    std::stringstream logFilePathAsStream{};
    logFilePathAsStream << logFileDirPathAsStream.str() << logFileNameAsStream.str();
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