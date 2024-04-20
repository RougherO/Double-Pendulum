cmake_minimum_required(VERSION 3.21.0)

# set output filename here
set(NAME_TARGET DoublePendulum)
project(${NAME_TARGET} LANGUAGES CXX)
# ----------------------------

# set all project files here
set(FILES_TARGET src/main.cpp src/App/App.cpp src/DoublePendulum/DPendulum.cpp)
add_executable(${NAME_TARGET} ${FILES_TARGET})
# ----------------------------

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

set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
add_subdirectory(submodules/SFML SFML/build/)


# set lib files here
set(LIB_FILES sfml-graphics sfml-window sfml-system)
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
