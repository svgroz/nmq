project(app)

add_executable(app)
target_sources(
    app
    PUBLIC 
    src/nmq.cpp
)

target_link_libraries(
    app 
    PUBLIC 
    core
)
