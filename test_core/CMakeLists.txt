project(test_core)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    test_core
    src/nmq/core/index_log.cpp
    #src/nmq/core/key_value.cpp
    #src/nmq/core/message.cpp
    #src/nmq/core/partition_log.cpp
)
target_link_libraries(
    test_core
    core
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_core)