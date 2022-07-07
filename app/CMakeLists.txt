project(app)

add_executable(app)
target_sources(
    app
    PUBLIC 
    configuration.h configuration.cpp
    hash.h hash.cpp
    nmq.cpp
    partition.h partition.cpp
    partition_log.h partition_log.cpp
    service_impl.h service_impl.cpp
    topic.h topic.cpp
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
