cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")
set(CMAKE_COLOR_DIAGNOSTICS ON)

project(idkernel)
ENABLE_LANGUAGE(ASM_NASM)

set(IDKERNEL_LIBRARIES "")
set(IDKERNEL_SOURCES "")


include_directories("${CMAKE_SOURCE_DIR}/external/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libstim/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libkernel/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/lib/libc++/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libkernel/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc/")
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/libc++/")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/kernel/")


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

target_link_libraries(
    ${PROJECT_NAME}

    crti_o
    # ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/crtbegin.o

    idk_libc
    idk_libc++
    ${IDKERNEL_LIBRARIES}

    # ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/crtend.o
    crtn_o
)

