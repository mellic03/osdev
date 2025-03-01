cmake_minimum_required(VERSION 3.31)

string(ASCII 27 Esc)
set(Green "${Esc}[32m")


if(KS_ARCH)
    message("${Green}Supplied KS_ARCH as " ${KS_ARCH})
else()
    message(FATAL_ERROR "KS_ARCH must be supplied!")
endif()


if(${KS_ARCH} STREQUAL "i686")
    message("${Green}Compiling for i686")
    set(ARCH_i686 true)
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
    set(CMAKE_CXX_FLAGS "-std=c++17 -ffreestanding -O2 -Wall -Wextra -Werror -fno-exceptions -fno-rtti -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2")
    
elseif(${KS_ARCH} STREQUAL "x86_64")
    message("${Green}Compiling for x86_64")
    set(ARCH_x86_64 true)
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
    # set(CMAKE_CXX_FLAGS "-std=c++17 -m64 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2")
    set(CMAKE_CXX_FLAGS "-std=c++17 -ffreestanding -O2 -Wall -Wextra -Werror -fno-exceptions -fno-rtti -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2")
endif()


set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR ${KS_ARCH})
set(TOOLS $ENV{HOME}/opt/cross)
set(CMAKE_C_COMPILER /bin/${KS_ARCH}-elf-gcc)
set(CMAKE_CXX_COMPILER /bin/${KS_ARCH}-elf-g++)
set(CMAKE_ASM_NASM_COMPILER /bin/nasm)
set(CMAKE_C_FLAGS "-std=c99 -ffreestanding -O2 -Wall -Wextra")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arch/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/system/linker-${KS_ARCH}.ld -ffreestanding -O2 -nostdlib -lgcc")
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/system/linker.ld -ffreestanding -O2 -nostdlib -lgcc")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(RootProject)
ENABLE_LANGUAGE(ASM_NASM)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/system/)


add_subdirectory(${CMAKE_SOURCE_DIR}/lib/)

add_subdirectory(${CMAKE_SOURCE_DIR}/system/kcommon/)
add_subdirectory(${CMAKE_SOURCE_DIR}/system/loader/)
add_subdirectory(${CMAKE_SOURCE_DIR}/system/kernel/)

