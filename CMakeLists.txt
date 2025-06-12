cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(CringeOS)
ENABLE_LANGUAGE(ASM_NASM)

# set(GlowOS_LIBRARIES "")

set(__x86_64__, "")
# set(KERNEL_SYSROOT_DIR "${CMAKE_SOURCE_DIR}/sysroot")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/usr/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/usr/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/usr/bin")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

include(${CMAKE_SOURCE_DIR}/config.cmake)

include_directories("${CMAKE_SOURCE_DIR}/external/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc++/include/")
# include_directories("${CMAKE_SOURCE_DIR}/lib/libmath/include/")

include_directories("${CMAKE_SOURCE_DIR}/kernel/include/")
include_directories("${CMAKE_SOURCE_DIR}/kernel/include/arch/x86_64/")
include_directories("${CMAKE_SOURCE_DIR}/include/")


add_compile_definitions("-D__is_kernel")

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc++/")
add_subdirectory("${CMAKE_SOURCE_DIR}/kernel/")
