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

#add_executable(clean_pacman_cache_dir main.cpp Package.cpp Package.h PackageFile.cpp PackageFile.h IgnoredPackageName.cpp IgnoredPackageName.h)
add_executable(clean_pacman_cache_dir main.cpp Package.cpp Package.h PackageFile.cpp PackageFile.h IgnoredPackageName.cpp IgnoredPackageName.h IgnoredPackageNamesEqualityComparator.h)
