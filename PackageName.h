#pragma once

#include <memory>
#include <string>
#include <ostream>

class PackageName {
public:
    explicit PackageName(std::string& name);

    std::string getName() const;

    bool operator<(const PackageName& packageName) const {
        return this->getName() < packageName.getName();
    }

    friend std::ostream &operator<<(std::ostream &os, const PackageName &packageName) {
        os << packageName.name;
        return os;
    }

private:
    std::string name;
};

namespace std {
    template<>
    struct less<unique_ptr<PackageName>> {
        bool operator() (const unique_ptr<PackageName>& lhs, const unique_ptr<PackageName>& rhs) const {
            return *lhs < *rhs;
        }
    };
}
