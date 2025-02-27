cmake_minimum_required(VERSION 3.31)

add_library(
    libKERNEL STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/module.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/memory/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/linear_allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/stack_allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/terminal/terminal.cpp
)

