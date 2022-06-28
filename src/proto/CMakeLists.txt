cmake_minimum_required (VERSION 3.20)

project (proto)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS nmq_server.proto)
protobuf_generate_python(PROTO_PY foo.proto)
add_library(proto ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proto ${Protobuf_LIBRARIES})

target_compile_features(proto PUBLIC cxx_std_11)
target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})