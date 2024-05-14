cmake_minimum_required(VERSION 3.21.0)

# set output filename here
set(NAME_TARGET DoublePendulum)
project(${NAME_TARGET} LANGUAGES CXX)
# ----------------------------

# set all project files here
set(FILES_TARGET src/main.cpp src/App/App.cpp src/DoublePendulum/DPendulum.cpp)
add_executable(${NAME_TARGET} ${FILES_TARGET})
# ----------------------------

# set font directory by passing the directory path to FONT_DIR macro 
target_compile_definitions(${NAME_TARGET} PRIVATE FONT_DIR="${CMAKE_SOURCE_DIR}/src/Fonts/")

### -fsanitize=.. needs to be passed both in compilation and linking stage
# enable/disable sanitizer here
set(ENABLE_UB_SANITIZER OFF)
# ----------------------------

# set compiler options here
set(COMPILE_OPTIONS -std=c++20 -Wall -Wextra -Wconversion -Wshadow -Wpedantic $<$<BOOL:${ENABLE_UB_SANITIZER}>:-fsanitize=undefined -fsanitize=address> -save-temps)
target_compile_options(${NAME_TARGET} PRIVATE ${COMPILE_OPTIONS})
# ----------------------------

# set linker options here
set(LINK_OPTIONS_TARGET $<$<BOOL:${ENABLE_UB_SANITIZER}>:-fsanitize=undefined -fsanitize=address>)
target_link_options(${NAME_TARGET} PRIVATE ${LINK_OPTIONS_TARGET})
# ----------------------------

# Load all required external libraries
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/submodules/SFML/CMakeLists.txt)
  message("${CMAKE_CURRENT_SOURCE_DIR}/SFML/CMakeLists.txt")
  message(FATAL_ERROR "Please initialize submodules using:\n  git submodule update --init --recursive")
endif()
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/SFML SFML/build/)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/submodules/ImGui/)

set(IMGUI_SFML_FIND_SFML OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/ImGui-SFML ImGui-SFML/build)
# ----------------------------

# set lib files here
set(LIB_FILES sfml-graphics sfml-window sfml-system ImGui-SFML)
target_link_libraries(${NAME_TARGET} ${LIB_FILES})
# ----------------------------

# # set include path here
# set(INCLUDE_PATH_TARGET "${CMAKE_SOURCE_DIR}/include")
# target_include_directories(${NAME_TARGET} PRIVATE ${INCLUDE_PATH_TARGET})
# # ----------------------------

# # set lib path here
# set(LIB_PATH_TARGET "${CMAKE_SOURCE_DIR}/lib")
# target_link_directories(${NAME_TARGET} PRIVATE ${LIB_PATH_TARGET})
# # ----------------------------
