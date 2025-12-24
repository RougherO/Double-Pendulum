add_library(pendulum STATIC
    sprite.cpp
    state.cpp
)

target_include_directories(pendulum PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(pendulum
    PUBLIC
        SFML::Graphics
)