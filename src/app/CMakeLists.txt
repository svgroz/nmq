cmake_minimum_required (VERSION 3.20)

project(app)

find_package(OpenSSL REQUIRED) 
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})

add_executable(app nmq.cpp)
target_link_libraries(app PUBLIC proto ${Protobuf_LIBRARIES} gRPC::grpc gRPC::grpc++)

target_compile_features(app PUBLIC cxx_std_11)
