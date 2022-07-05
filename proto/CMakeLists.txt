project (proto)

find_package(Protobuf 3.15 REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(proto nmq_server.proto)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET proto LANGUAGE cpp)
protobuf_generate(TARGET proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")


target_link_libraries(proto ${Protobuf_LIBRARIES})

target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})