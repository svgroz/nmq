project(test_core)

add_executable(test_core)
target_sources(
    test_core
    PUBLIC
    hash_test.cpp
)
target_include_directories(test_core PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(test_core PRIVATE "BOOST_TEST_DYN_LINK=1")
target_link_libraries(test_core core Boost::unit_test_framework)

add_test(NAME test1 COMMAND test_core)