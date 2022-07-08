cmake_minimum_required (VERSION 3.20)

project(nmq)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Protobuf 3.15 REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED) 
find_package(Boost COMPONENTS log unit_test_framework REQUIRED)

add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(proto)
add_subdirectory(test_core)
