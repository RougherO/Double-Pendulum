# Global properties
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Compiler warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -pedantic -Wshadow -Wconversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Add options for MSVC
endif()