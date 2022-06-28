cmake_minimum_required (VERSION 3.20)

project (nmq)

add_subdirectory(src/app)
add_subdirectory(src/proto)

#find_package(Protobuf REQUIRED)
#include_directories(${Protobuf_INCLUDE_DIRS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS src/proto/nmq_server.proto)

#add_library(nmq-proto ${PROTO_SRCS} ${PROTO_HDRS})
#target_compile_features(nmq-proto PUBLIC cxx_std_11)

#add_executable(nmq-app
#                    ${PROTO_SRCS} ${PROTO_HDRS}
#                    src/app/nmq.cpp 
#                    src/app/catalogue.h src/app/catalogue.cpp
#)

#target_compile_features(nmq-app PUBLIC cxx_std_11)
#target_link_libraries(nmq-app
#  ${Protobuf_LIBRARIES}
#  nmq-proto
#)
