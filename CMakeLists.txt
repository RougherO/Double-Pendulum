cmake_minimum_required(VERSION 3.21.0)
project(DoublePendulum VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)

FetchContent_MakeAvailable(SFML)

add_executable(DoublePendulum "src/main.cpp" "src/App/App.cpp" "src/DoublePendulum/DPendulum.cpp")
target_link_libraries(DoublePendulum PRIVATE sfml-graphics sfml-window sfml-system)

if (WIN32) 
    add_custom_command(
        TARGET DoublePendulum
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<BOOL:${ARCH_64BITS}>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:DoublePendulum>
        VERBATIM
    )
endif()