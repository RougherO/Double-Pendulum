cmake_minimum_required(VERSION 3.20..4.0)
project(DoublePendulum LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(common)
include(sanitizers)

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/submodules/SFML/CMakeLists.txt)
  message(FATAL_ERROR "Please initialize submodules using:\n  git submodule update --init --recursive")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/SFML)

add_subdirectory(src)