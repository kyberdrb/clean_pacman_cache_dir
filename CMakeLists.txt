cmake_minimum_required(VERSION 3.21)
project(clean_pacman_cache_dir)

set(CMAKE_CXX_STANDARD 17)

# Use compiler specific commands to link the 'alpm' library for UNIX compilers, e .g. clang, clang++
#  in order to fix a warning message "clang-13: warning: -lalpm: 'linker' input unused [-Wunused-command-line-argument]"
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Using GNU C++ compiler")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lalpm -laudit")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message("using UNIX/Apple Clang C++ compiler")
    link_libraries(alpm)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message("using Visual Studio C++ compiler")
endif()

# Enable code coverage
#  Comment out to prevent runtime error messages like e. g.: "libgcov profiling error:/home/laptop/git/kyberdrb/clean_pacman_cache_dir/cmake-build-debug/CMakeFiles/clean_pacman_cache_dir.dir/MoverOfInstallationPackageFiles.cpp.gcda:overwriting an existing profile data with a different timestamp"
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g")

message("All used compiler flags: ${CMAKE_CXX_FLAGS}")

add_executable(clean_pacman_cache_dir src/main.cpp src/ExtendedInstallationPackageFile.cpp src/ExtendedInstallationPackageFile.h src/IgnoredPackageName.cpp src/IgnoredPackageName.h src/PackageName.cpp src/PackageName.h src/PackageVersion.cpp src/PackageVersion.h src/SimpleInstallationPackageFile.cpp src/SimpleInstallationPackageFile.h src/AbsolutePath.cpp src/AbsolutePath.h src/FileMoverSingleton.cpp src/FileMoverSingleton.h src/Filename.cpp src/Filename.h src/SimpleInstallationPackageFileType.h src/Package.h src/LocallyInstalledPackage.h src/PackageWithInferredName.h src/PackageWithInferredName.cpp src/LocallyInstalledPackage.cpp src/PackageNameAndVersion.cpp src/PackageNameAndVersion.h src/Package.cpp src/IgnoredPackageNames.cpp src/IgnoredPackageNames.h src/IgnoredPackageNameComparatorPredicate.h src/LocallyInstalledPackages.cpp src/LocallyInstalledPackages.h src/PackageNameMissing.h src/PackageNameMissing.cpp src/MatchFinderForPackageFilesToLocallyInstalledPackages.cpp src/MatchFinderForPackageFilesToLocallyInstalledPackages.h src/MoverOfInstallationPackageFiles.cpp src/MoverOfInstallationPackageFiles.h src/TerminalAndLoggerSingleton.cpp src/TerminalAndLoggerSingleton.h)
