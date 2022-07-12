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
    key_value
    src/nmq/core/key_value.cpp
    src/nmq/core/message.cpp
)
target_link_libraries(
    key_value
    core
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(key_value)