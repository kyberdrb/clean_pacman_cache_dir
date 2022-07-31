//
// Created by laptop on 6/18/22.
//

#include "PackageNameMissing.h"

PackageNameMissing::PackageNameMissing(const Package& packageWithInferredName) :
        inferredPackageName(packageWithInferredName.getName())
{
    std::stringstream exceptionMessage;
    exceptionMessage << "There are zero entries for the package name: '" << inferredPackageName << "'";
    this->exceptionMessageAsCharSequence = exceptionMessage.str();
}
