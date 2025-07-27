cmake_minimum_required(VERSION 3.20)
project(physync_shared)

add_library(physync_shared INTERFACE)

target_include_directories(physync_shared INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/client_server_shared
    ${CMAKE_CURRENT_SOURCE_DIR}/client_server_shared/external
)

add_library(physync::shared ALIAS physync_shared)