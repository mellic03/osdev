cmake_minimum_required(VERSION 3.15)
project(ckos)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")

if(${KERNEL_ARCH} STREQUAL "i686")
    message("${Green} Compiling for i686")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
elseif(${KERNEL_ARCH} STREQUAL "x86_64")
    message("${Green} Compiling for x86_64")
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
else()
    message(FATAL_ERROR "-DKERNEL_ARCH must be either i686 or x86_64")
endif()


set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ${KERNEL_ARCH})
set(TOOLS $ENV{HOME}/opt/cross)
set(CMAKE_C_COMPILER /bin/${KERNEL_ARCH}-elf-gcc)
set(CMAKE_CXX_COMPILER /bin/${KERNEL_ARCH}-elf-g++)
set(CMAKE_ASM_NASM_COMPILER nasm)
ENABLE_LANGUAGE(ASM_NASM)


# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS "-std=gnu99 -ffreestanding -O2 -Wall -Wextra")


set(KERNEL_BIN ${PROJECT_NAME}.bin)
set(LOADER_BIN ${PROJECT_NAME}.bin)


# Include directories
# ----------------------------------------------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/src/kernel/include/)
include_directories(${CMAKE_SOURCE_DIR}/src/lib/)
# ----------------------------------------------------------------------------------------------



execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=crtbegin.o
    OUTPUT_VARIABLE CRTBEGIN_OBJ
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=crtend.o
    OUTPUT_VARIABLE CRTEND_OBJ
    OUTPUT_STRIP_TRAILING_WHITESPACE
)



# Build kernel
# ----------------------------------------------------------------------------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/src/boot/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/kernel/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/libc/)

add_executable(
    ${KERNEL_BIN}
    ${CMAKE_SOURCE_DIR}/src/dummy.cpp
    ${CMAKE_SOURCE_DIR}/src/boot/boot-${KERNEL_ARCH}.asm
    ${CMAKE_SOURCE_DIR}/src/boot/boot.cpp
)

target_link_libraries(
    ${KERNEL_BIN}
    # /usr/lib/gcc/i686-elf/11.2.0/crtbegin.o
    # ${CMAKE_SOURCE_DIR}/src/kernel/ctor/crti.o
    # libBOOT
    libKERNEL
    libc
    # /usr/lib/gcc/i686-elf/11.2.0/crtend.o
    # ${CMAKE_SOURCE_DIR}/src/kernel/ctor/crtn.o
)

# Set linker properties
set_target_properties(
    ${KERNEL_BIN} PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/src/linker-${KERNEL_ARCH}.ld -ffreestanding -O2 -nostdlib -lgcc"
)

# ----------------------------------------------------------------------------------------------


