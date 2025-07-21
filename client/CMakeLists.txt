cmake_minimum_required(VERSION 3.10)

project(physync_client)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("include")

find_library(LIB_GLFW3 glfw3 PATH ./lib)
if(NOT LIB_GLFW3)
    message(FATAL_ERROR "Could not find the GLFW3 library")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
add_executable(physync_client ${SOURCES})

target_link_libraries(physync_client ${LIB_GLFW3})
