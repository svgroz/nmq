project(core)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(core)
target_sources(
    core
    PUBLIC
    src/nmq/core/configuration.cpp include/nmq/core/configuration.h
    src/nmq/core/partition.cpp include/nmq/core/partition.h
    src/nmq/core/partition_log.cpp include/nmq/core/partition_log.h
    src/nmq/core/service_impl.cpp include/nmq/core/service_impl.h
    src/nmq/core/topic.cpp include/nmq/core/topic.h
)
target_include_directories(
    core
    PUBLIC
    include
)
target_link_libraries(
    core 
    PUBLIC 
    proto
    ${Protobuf_LIBRARIES} 
    Boost::log
)
