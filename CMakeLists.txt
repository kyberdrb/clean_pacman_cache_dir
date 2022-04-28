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
    # Adding '-fuse-ld=lld' in 'CMAKE_CXX_FLAGS' invokes the LLVM linker
    #  but it produces a warning "clang-13: warning: argument unused during compilation: '-fuse-ld=lld' [-Wunused-command-line-argument]"
    #  because the flag is not used during compilation, yet after compilation, as the compiler forwards the compiled object files to the linker i. e. our custom LLVM linker
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
    link_libraries(alpm)

    # I also tried to use these compiler options for CMake
    #   -DCMAKE_LINKER=/usr/bin/ld.lld -DLINK_LIBRARIES="lalpm" -DCMAKE_CXX_LINK_EXECUTABLE="<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
    #  but the compilation stopped at the linking, because the linker didn't find 'ifstream' library. Maybe explicitly specifying compilation and linking language helps?
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message("using Visual Studio C++ compiler")
endif()

add_executable(clean_pacman_cache_dir main.cpp Package.cpp Package.h PackageFile.cpp PackageFile.h PackageName.cpp PackageName.h PackageVersion.cpp PackageVersion.h)
