add_library(physics STATIC
    modified_verlet.cpp
    runge_kutta.cpp
)

target_include_directories(physics PUBLIC ${CMAKE_SOURCE_DIR}/include)