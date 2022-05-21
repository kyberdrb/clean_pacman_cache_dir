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

    friend bool operator==(const std::unique_ptr<IgnoredPackageName>& oneIgnoredPackageName, const std::unique_ptr<IgnoredPackageName>& anotherIgnoredPackageName) {
        return oneIgnoredPackageName->name == anotherIgnoredPackageName->name;
    }

private:
    std::string name;
};
