cmake_minimum_required(VERSION 3.10)

project(physync_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("include")
add_subdirectory(../shared physync_shared_build)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_library(LIB_RP3D reactphysics3d_debug PATH ./lib)
else()
    find_library(LIB_RP3D reactphysics3d PATH ./lib)
endif()
if(NOT LIB_RP3D)
    message(FATAL_ERROR "Could not find the reactphysics3d library")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
add_executable(physync_server ${SOURCES})

target_link_libraries(physync_server ${LIB_RP3D})
target_link_libraries(physync_server physync::shared)
