project(app)

find_package(OpenSSL REQUIRED) 
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})

add_executable(app 
                configuration.h configuration.cpp
                nmq.cpp
                partition.h partition.cpp
                service_impl.h service_impl.cpp
                topic.h topic.cpp
                utils.h
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
