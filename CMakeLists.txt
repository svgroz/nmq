cmake_minimum_required (VERSION 3.20)

project(nmq)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost COMPONENTS log REQUIRED)

add_subdirectory(core)
add_subdirectory(test_core)
