project(test_core)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(test_core)
target_sources(
    test_core
    PUBLIC
    src/nmq/core/message.cpp
)
target_include_directories(test_core PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(test_core PRIVATE "BOOST_TEST_DYN_LINK=1")
target_link_libraries(test_core core Boost::unit_test_framework)

add_test(NAME example COMMAND test_core)