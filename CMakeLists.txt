cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(idkernel)
ENABLE_LANGUAGE(ASM_NASM)

set(IDKERNEL_LIBRARIES "")
set(IDKERNEL_SOURCES "")

set(KERNEL_SYSROOT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/sysroot/bin")
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")


include(${CMAKE_SOURCE_DIR}/toolchain.cmake)

include_directories("${CMAKE_SOURCE_DIR}/external/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libk/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc++/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libk/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc++/")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/kernel/")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/programs/")


add_library(
    crti_o OBJECT
    ${CMAKE_SOURCE_DIR}/src/kernel/abi/crti.asm
)


add_library(
    crtn_o OBJECT
    ${CMAKE_SOURCE_DIR}/src/kernel/abi/crtn.asm
)






add_executable(
    ${PROJECT_NAME}
    ${IDKERNEL_SOURCES}
)


target_link_options(
    ${PROJECT_NAME} PRIVATE
    -Wl,-m,elf_x86_64
    -Wl,--build-id=none
    -nostdlib
    -static
    -mgeneral-regs-only
    -zmax-page-size=0x1000
    -mno-red-zone
    -T${CMAKE_SOURCE_DIR}/src/linker.ld
)


target_compile_definitions(
    ${PROJECT_NAME} PUBLIC 
    -D__is_kernel
)

target_link_libraries(
    ${PROJECT_NAME}

    crti_o
    # ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/crtbegin.o

    libc_k
    libc++
    ${IDKERNEL_LIBRARIES}

    # ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/crtend.o
    crtn_o
)


