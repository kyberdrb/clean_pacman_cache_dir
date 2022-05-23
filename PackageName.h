//
// Created by laptop on 5/23/22.
//

#pragma once

#include <memory>
#include <ostream>

class PackageName {
public:
    explicit PackageName(std::string name);

    bool empty() const;
    const char& at(size_t position) const;
    size_t size() const;
    void pop_back();
    const std::string& string() const;
//    std::string string() const;

    friend std::ostream& operator<<(std::ostream& out, const PackageName& packageName) {
        out << packageName.name;
        return out;
    }

// WORKS for direct unique_ptr comparison
//    friend bool operator<(
//            const std::unique_ptr<PackageName>& packageName,
//            const std::unique_ptr<PackageName>& otherPackageName)
//    {
//        return packageName->name < otherPackageName->name;
//    }

// WORKS for direct unique_ptr comparison
//    friend bool operator<(
//            std::unique_ptr<PackageName>& packageName,
//            std::unique_ptr<PackageName>& otherPackageName)
//    {
//        return packageName->name < otherPackageName->name;
//    }

// WORKS with dereferenced comparison
//    friend bool operator<(
//            PackageName& packageName,
//            PackageName& otherPackageName)
//    {
//        return packageName.name < otherPackageName.name;
//    }

// WORKS with dereferenced comparison
//    friend bool operator<(
//            const PackageName& packageName,
//            const PackageName& otherPackageName)
//    {
//        return packageName.name < otherPackageName.name;
//    }

// WORKS for direct unique_ptr comparison for 'std::greater' to order elements descendingly
//    friend bool operator>(
//            const std::unique_ptr<PackageName>& packageName,
//            const std::unique_ptr<PackageName>& otherPackageName)
//    {
//        return packageName->name > otherPackageName->name;
//    }

// WORKS with dereferenced comparison for 'std::greater' to order elements descendingly
//    friend bool operator>(
//            const PackageName& packageName,
//            const PackageName& otherPackageName)
//    {
//        return packageName.name > otherPackageName.name;
//    }

private:
    std::string name;
};
