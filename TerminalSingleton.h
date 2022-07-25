//
// Created by laptop on 7/25/22.
//

#pragma once

#include <memory>
#include <sstream>

class TerminalSingleton {
private:
    class PrivateAuthorizationToken {
    public:
        // make the Singleton class a 'friend' of the Token class
        //  in order to make the private constructor of the Token class accessible for the Singleton class and for it only
        friend TerminalSingleton;

    private:
        PrivateAuthorizationToken() = default;
    };

public:
    explicit TerminalSingleton(const PrivateAuthorizationToken& privateAuthorizationToken);

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
    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance;

    TerminalSingleton() = default;
};
