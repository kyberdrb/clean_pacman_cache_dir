cmake_minimum_required(VERSION 3.21)
project(clean_pacman_cache_dir)

set(CMAKE_CXX_STANDARD 17)

# Use compiler specific commands to link the 'alpm' library for UNIX compilers, e .g. clang, clang++
#  in order to fix a warning message "clang-13: warning: -lalpm: 'linker' input unused [-Wunused-command-line-argument]"
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Using GNU C++ compiler")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lalpm")
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

add_executable(clean_pacman_cache_dir main.cpp ExtendedInstallationPackageFile.cpp ExtendedInstallationPackageFile.h IgnoredPackageName.cpp IgnoredPackageName.h PackageName.cpp PackageName.h PackageVersion.cpp PackageVersion.h SimpleInstallationPackageFile.cpp SimpleInstallationPackageFile.h AbsolutePath.cpp AbsolutePath.h FileMover.cpp FileMover.h Filename.cpp Filename.h SimpleInstallationPackageFileType.h Package.h LocallyInstalledPackage.h PackageWithInferredName.h PackageWithInferredName.cpp LocallyInstalledPackage.cpp PackageNameAndVersion.cpp PackageNameAndVersion.h Package.cpp IgnoredPackageNames.cpp IgnoredPackageNames.h IgnoredPackageNameComparatorPredicate.h LocallyInstalledPackages.cpp LocallyInstalledPackages.h PackageNameMissing.h PackageNameMissing.cpp MatchFinderForPackageFilesToLocallyInstalledPackages.cpp MatchFinderForPackageFilesToLocallyInstalledPackages.h MoverOfInstallationPackageFiles.cpp MoverOfInstallationPackageFiles.h)
