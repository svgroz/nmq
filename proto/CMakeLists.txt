project(proto)

add_library(proto)
target_sources(proto PUBLIC nmq_server.proto)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET proto LANGUAGE cpp)
protobuf_generate(TARGET proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
target_include_directories(
    proto 
    PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${Protobuf_INCLUDE_DIRS}
)
target_link_libraries(proto ${Protobuf_LIBRARIES})
