cmake_minimum_required (VERSION 3.20)

project (nmq)
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

#---------------------------------------------------------------------------
add_library(
    proto 
    proto/nmq_server.proto
)
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
#---------------------------------------------------------------------------
add_executable(
    app 
    app/configuration.h app/configuration.cpp
    app/hash.h app/hash.cpp
    app/nmq.cpp
    app/partition.h app/partition.cpp
    app/partition_log.h app/partition_log.cpp
    app/service_impl.h app/service_impl.cpp
    app/topic.h app/topic.cpp
)
target_include_directories(
    app
    PUBLIC
    ${Protobuf_INCLUDE_DIRS}
)
target_link_libraries(
    app 
    PUBLIC 
    proto
    ${Protobuf_LIBRARIES} 
    gRPC::grpc 
    gRPC::grpc++ 
    Boost::log
)
#---------------------------------------------------------------------------
add_executable(
    test_app
    test_app/hash_test.cpp
)
# indicates the include paths
target_include_directories(test_app PRIVATE ${Boost_INCLUDE_DIRS})
# indicates the shared library variant
target_compile_definitions(test_app PRIVATE "BOOST_TEST_DYN_LINK=1")
# indicates the link paths
target_link_libraries(test_app ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# declares a test with our executable
add_test(NAME test1 COMMAND test_app)
#---------------------------------------------------------------------------
