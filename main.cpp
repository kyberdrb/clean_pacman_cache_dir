#include "Packages.h"

#include <memory>

int main() {
    auto packages = std::make_unique<Packages>();
    packages->cleanCachedFilesOfPackageManagers();

    return 0;
}
