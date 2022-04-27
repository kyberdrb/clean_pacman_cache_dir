cmake_minimum_required(VERSION 3.21)
project(clean_pacman_cache_dir)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lalpm")

add_executable(clean_pacman_cache_dir main.cpp Package.cpp Package.h PackageFile.cpp PackageFile.h)
