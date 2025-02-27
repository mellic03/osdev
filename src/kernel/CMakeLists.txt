cmake_minimum_required(VERSION 3.31)

add_library(
    libKernel STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp
)

target_link_libraries(
    libKernel
    libc
)