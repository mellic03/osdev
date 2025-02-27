cmake_minimum_required(VERSION 3.31)

add_library(
    libLoader STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/boot.asm
)

target_link_libraries(
    libLoader
    libc
    libKernelCommon
)