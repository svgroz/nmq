project(core)

enable_testing()

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
  #  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(core SHARED)
target_sources(
    core
    PUBLIC
    src/nmq/core/index_log.cpp
    src/nmq/core/key_value.cpp
    src/nmq/core/message.cpp
)
target_include_directories(
    core
    PUBLIC
    include
)
target_link_libraries(
    core 
    PUBLIC 
    spdlog::spdlog
    fmt::fmt
)
