cmake_minimum_required (VERSION 3.20)

project(app)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

add_executable(app nmq.cpp)
target_link_libraries(app PUBLIC proto ${Protobuf_LIBRARIES})

target_compile_features(app PUBLIC cxx_std_11)
