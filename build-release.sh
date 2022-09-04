#!/bin/sh

set -x

PROJECT_DIR="$(dirname "$(readlink --canonicalize "$0")")"

# Clean previous executable
/usr/bin/cmake --build "${PROJECT_DIR}/cmake-build-release/" --target clean

# Generate makefiles for building the 'Release' version of the binary executable
/usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -G Ninja -S "${PROJECT_DIR}" -B "${PROJECT_DIR}/cmake-build-release/"

# Build the 'Release' version of the binary executable
PROJECT_NAME="$(basename "${PROJECT_DIR}")"
/usr/bin/cmake --build "${PROJECT_DIR}/cmake-build-release" --target "${PROJECT_NAME}"

echo
echo "The 'Release' version of the binary executable for '${PROJECT_NAME}' can be run with command"
echo "    '${PROJECT_DIR}/cmake-build-release/${PROJECT_NAME}'"

