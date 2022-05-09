//
// Created by laptop on 5/7/22.
//

#pragma once

#include <memory>
#include <string>

class IgnoredPackageName {
public:
    explicit IgnoredPackageName(std::string ignoredPackageName);

    friend std::ostream& operator<<(std::ostream& out, const IgnoredPackageName& ignoredPackageName) {
        out << ignoredPackageName.name;
        return out;
    }

// FOR DIRECT COMPARISON OF SMART POINTERS

    // Fails to find an element in a vector of unique pointers, because the parameters are not constant
    //  because the third parameter in 'std::find' takes a reference to a constant type 'const _Tp& __val'
    //  and in this function are defined parameters as references to the type of element the vector holds
    //  not as reference to the constant element type that the vector holds,
    //  therefore the types for parameters for the equality operator in the class
    //  and the type of the third parameter in the 'std::find'
    //  need to match in their constant modifiers in order to accurately find an element in the vector,
    //  even though the type of elements the vector holds are NOT constant
//    friend bool operator==(std::unique_ptr<IgnoredPackageName>& oneIgnoredPackageName, std::unique_ptr<IgnoredPackageName>& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName->name == anotherIgnoredPackageName->name;
//    }

    // Fails to find an element in a vector of unique pointers, because the 'std::find' needs to have access to the
    //  internal structure for the type of elements it compares
    //  therefore the equality operator needs to be declared as public 'friend' i.e. nonmember function
    //  even though it is defined in the namespace of this class
//    bool operator==(const std::unique_ptr<IgnoredPackageName>& otherIgnoredPackageName) const {
//        return this->name == otherIgnoredPackageName->name;
//    }

    // Successfully finds an element in a vector of unique pointers because
    //  1. the 'std::find' function has access to the internal structure of the elements that it compares
    //     for equality with the equality operator 'operator=='
    //  2. and the types for parameters for the equality operator in the class
    //     and the type of the third parameter in the 'std::find'
    //     are matching in their constant modifiers for the type of elements the vector holds
//    friend bool operator==(const std::unique_ptr<IgnoredPackageName>& oneIgnoredPackageName, const std::unique_ptr<IgnoredPackageName>& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName->name == anotherIgnoredPackageName->name;
//    }

// FOR COMPARISON OF DEREFERENCED (SMART) POINTERS (I. E. COMPARING OBJECTS not pointers to objects nor pointers to pointers)

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    friend bool operator==(IgnoredPackageName& oneIgnoredPackageName, IgnoredPackageName& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName.name == anotherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    friend bool operator==(const IgnoredPackageName& oneIgnoredPackageName, const IgnoredPackageName& anotherIgnoredPackageName) {
//        return oneIgnoredPackageName.name == anotherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(const IgnoredPackageName& otherIgnoredPackageName) const {
//        return this->name == otherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(const IgnoredPackageName& otherIgnoredPackageName) {
//        return this->name == otherIgnoredPackageName.name;
//    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
    bool operator==(IgnoredPackageName& otherIgnoredPackageName) const {
        return this->name == otherIgnoredPackageName.name;
    }

    // Successfully finds an element in a vector of unique pointers for dereferenced unique_ptrs
//    bool operator==(IgnoredPackageName& otherIgnoredPackageName) {
//        return this->name == otherIgnoredPackageName.name;
//    }

private:
    std::string name;
};
