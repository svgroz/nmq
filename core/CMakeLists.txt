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
    src/nmq/core/configuration.cpp 
    src/nmq/core/header.cpp
    src/nmq/core/message.cpp
    src/nmq/core/partition.cpp 
    src/nmq/core/partition_log.cpp
    src/nmq/core/topic.cpp 
)
target_include_directories(
    core
    PUBLIC
    include
)
target_link_libraries(
    core 
    PUBLIC 
    Boost::log
)
