project(test_app)

# creates the executable
add_executable(test_app)
target_sources(
    test_app
    PUBLIC
    hash_test.cpp
)
target_include_directories(test_app PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_definitions(test_app PRIVATE "BOOST_TEST_DYN_LINK=1")
target_link_libraries(test_app Boost::unit_test_framework)

add_test(NAME test1 COMMAND test_app)