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

protected:
    // Define only the default constructor and make it hidden from public interface,
    //  so that only the Singleton class controls the creation and access to the single instance,
    //  but it's still visible for 'std::make_unique' and derived classes (for 'TerminalSingletonDerived'
    //  and or for possible testing/mocking purposes)
    // The constructor defined as 'protected' instead of 'private' to prevent errors:
    //   - g++: "‘TerminalSingletonDerived::TerminalSingletonDerived()’ is implicitly deleted because the default definition would be ill-formed:"
    //   - clang-tidy: "In template: call to implicitly-deleted default constructor of 'TerminalSingletonDerived'"
    //      - even after making the default constructor 'public' in 'TerminalSingletonDerived' the error still persists
    TerminalSingleton() = default;
};
