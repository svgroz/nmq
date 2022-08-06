cmake_minimum_required (VERSION 3.20)

project(nmq)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost REQUIRED)

add_subdirectory(core)
if (BUILD_TESTING)
    add_subdirectory(test_core)
endif()