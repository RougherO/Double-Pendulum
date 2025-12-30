set(SRC_FILES
    main.cpp
    simulator.cpp
)
add_executable(${PROJECT_NAME} ${SRC_FILES})

add_subdirectory(pendulum)
add_subdirectory(physics)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        pendulum
        physics
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/submodules/ImGui
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SFML::System
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ASSET_FOLDER="${CMAKE_SOURCE_DIR}/assets"
)