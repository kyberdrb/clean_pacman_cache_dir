//
// Created by laptop on 4/28/22.
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

    friend std::ostream& operator<<(std::ostream& out, const PackageName& packageName) {
        out << packageName.name;
        return out;
    }

    bool operator==(const PackageName& otherPackageName) const {
        return PackageName::name == otherPackageName.name;
    }

    bool operator<(const PackageName& otherPakcageName) const {
        return PackageName::name < otherPakcageName.name;
    }

private:
    std::string name;
};

// overload the 'less' functor in order to enable lookup ('find') in a 'set' or a 'map' with instances of this class as a key, or with any custom object-type key
namespace std {
    template<>
    struct less<unique_ptr<PackageName>> {
    bool operator() (const unique_ptr<PackageName>& onePackageName, const unique_ptr<PackageName>& otherPackageName) const {
        return *onePackageName < *otherPackageName;
    }
};
}