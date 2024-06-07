cmake_minimum_required(VERSION 3.27.0)

# set output filename here
set(NAME_TARGET DoublePendulum)
set(VERSION_TARGET 2.0.1)
project(${NAME_TARGET} LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/submodules/SFML/CMakeLists.txt)
  message("${CMAKE_SOURCE_DIR}/SFML/CMakeLists.txt")
  message(FATAL_ERROR "Please initialize submodules using:\n  git submodule update --init --recursive")
endif()
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/SFML SFML/build EXCLUDE_FROM_ALL)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/submodules/ImGui/)

set(IMGUI_SFML_FIND_SFML OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/ImGui-SFML ImGui-SFML/build EXCLUDE_FROM_ALL)
# ----------------------------

# set lib files here
set(LIB_FILES sfml-graphics sfml-window sfml-system ImGui-SFML)
target_link_libraries(${NAME_TARGET} ${LIB_FILES})
# ----------------------------

# CPack configuration
set(CPACK_GENERATOR ZIP TGZ)
set(CPACK_PACKAGE_NAME ${NAME_TARGET})
set(CPACK_PACKAGE_VERSION ${VERSION_TARGET})
set(CPACK_PACKAGE_CONTACT "roughero2@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Double Pendulum Simulation")
# Use the OS name for the package file name
if(DEFINED ENV{RUNNER_OS})
    string(TOLOWER "$ENV{RUNNER_OS}" OS_NAME)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${OS_NAME}")
else()
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
endif()
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-Source")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

include(CPack)

# Specify the install directories for the executable and libraries
install(TARGETS ${NAME_TARGET} DESTINATION bin)