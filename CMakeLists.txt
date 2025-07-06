cmake_minimum_required(VERSION 3.8)
project(nginy)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_C_FLAGS "-Wall")

# Change path from /src if needed, or add more directories
file(GLOB_RECURSE sources
        "${CMAKE_SOURCE_DIR}/src/*.c"
        )
# Add precompiler definitions like that:
add_definitions(-DSOME_DEFINITION)

add_executable(nginy ${sources})

# Add more include directories if needed
# target_include_directories(SQLike PUBLIC "${CMAKE_SOURCE_DIR}/src/includes")

# If you have precompiled headers you can add them like this
# target_precompiled_headers(my_app PRIVATE "${CMAKE_SOURCE_DIR}/src/pch.h")
