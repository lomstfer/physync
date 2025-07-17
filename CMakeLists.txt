cmake_minimum_required(VERSION 3.10)

project(physync)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("include")

find_library(LIB_GLFW3 glfw3 PATH ./lib)
if(NOT LIB_GLFW3)
    message(FATAL_ERROR "Could not find the GLFW3 library")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
add_executable(physync ${SOURCES})

target_link_libraries(physync ${LIB_GLFW3})
