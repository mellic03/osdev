cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(CringeOS)
ENABLE_LANGUAGE(ASM_NASM)

set(CringeOS_LIBRARIES "")
# set(CringeOS_SOURCES "")

set(__x86_64__, "")
set(KERNEL_SYSROOT_DIR ${CMAKE_SOURCE_DIR}/sysroot)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${KERNEL_SYSROOT_DIR}/usr/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${KERNEL_SYSROOT_DIR}/usr/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${KERNEL_SYSROOT_DIR}/usr/bin)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")


include(${CMAKE_SOURCE_DIR}/toolchain.cmake)

include_directories("${CMAKE_SOURCE_DIR}/external/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc++/include/")

include_directories("${CMAKE_SOURCE_DIR}/kernel/include/")
include_directories("${CMAKE_SOURCE_DIR}/kernel/include/arch/x86_64/")
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc++/")

add_subdirectory(${CMAKE_SOURCE_DIR}/kernel/)


add_executable(
    ${PROJECT_NAME}
    ${CringeOS_SOURCES}
)

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC 
    -D__is_kernel
)


# add_library(crti.o OBJECT ${CMAKE_SOURCE_DIR}/kernel/src/crti.asm)
# add_library(crtn.o OBJECT ${CMAKE_SOURCE_DIR}/kernel/src/crtn.asm)

target_link_libraries(
    ${PROJECT_NAME}
    libc_k
    libc++_k
    ${CringeOS_LIBRARIES}
)


