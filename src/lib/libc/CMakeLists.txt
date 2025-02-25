cmake_minimum_required(VERSION 3.31)

# message(STATUS "Processing libc directory: ${CMAKE_CURRENT_SOURCE_DIR}")


add_library(
    libc
    ${CMAKE_CURRENT_SOURCE_DIR}/stdio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/printf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/string.c
)

