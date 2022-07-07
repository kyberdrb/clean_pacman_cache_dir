//
// Created by laptop on 7/5/22.
//

#include "TerminalSingleton.h"

#include <iostream>

const TerminalSingleton& TerminalSingleton::get() {
    struct TerminalSingletonDerived : public TerminalSingleton {};

    // Derived class 'TerminalSingletonDerived' can be instantiated and destroyed entirely,
    //  even though it's in an inheritance relationship with 'TerminalSingleton'
    //  because the body of the derived class is emtpy - thus has no member functions or variables
    static std::unique_ptr<TerminalSingletonDerived> theOneAndOnlyTerminalSingletonDerivedInstance =
            std::make_unique<TerminalSingletonDerived>();

    return *theOneAndOnlyTerminalSingletonDerivedInstance;
}

//std::unique_ptr<TerminalSingleton> TerminalSingleton::theOneAndOnlyTerminalSingletonInstance;

//const TerminalSingleton& TerminalSingleton::get() {
//    // Making the 'std::make_unique' a friend of this class fixes the error message in clang-tidy:
//    //     "calling a private constructor of class 'TerminalSingleton'"
//    //  and its related error message from the compiler
//    //     "error: ‘constexpr TerminalSingleton::TerminalSingleton()’ is private within this context"
//    static std::unique_ptr<TerminalSingleton> theOneAndOnlyTerminalSingletonInstance = std::make_unique<TerminalSingleton>();
////    TerminalSingleton::theOneAndOnlyTerminalSingletonInstance = std::make_unique<TerminalSingleton>();
//
//    return *TerminalSingleton::theOneAndOnlyTerminalSingletonInstance;
//}

const TerminalSingleton& TerminalSingleton::printText(const std::string& text) const {
    std::cout << text;
    return *this;
}
