cmake_minimum_required(VERSION 3.21.0)
project(DoublePendulum VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)

# FetchContent_Declare(
#     ImGUI-SFML
#     GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
#     GIT_TAG 2.6.x
# )

FetchContent_MakeAvailable(SFML)

add_executable(DoublePendulum "src/main.cpp" "src/App/App.cpp" "src/DoublePendulum/DPendulum.cpp")
set_property(TARGET DoublePendulum PROPERTY CXX_STANDARD_REQUIRED 20)
target_link_libraries(DoublePendulum PRIVATE sfml-graphics sfml-window sfml-system)