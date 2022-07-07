cmake_minimum_required (VERSION 3.20)

project(nmq)
enable_testing()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Protobuf 3.15 REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED) 
find_package(Boost COMPONENTS log unit_test_framework REQUIRED)

add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(proto)
add_subdirectory(test_core)
