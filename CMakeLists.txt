cmake_minimum_required (VERSION 3.20)

project (nmq)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/app)
add_subdirectory(src/proto)
