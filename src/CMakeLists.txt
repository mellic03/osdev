cmake_minimum_required(VERSION 3.31)


string(ASCII 27 Esc)
set(Green "${Esc}[32m")


set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(TOOLS $ENV{HOME}/opt/cross)
set(CMAKE_C_COMPILER /bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER /bin/x86_64-elf-g++)
set(CMAKE_ASM_COMPILER /bin/x86_64-elf-as)
set(CMAKE_ASM_NASM_COMPILER /bin/nasm)
# set(CMAKE_EXE_LINKER /bin/x86_64-elf-ld)


set(IDK_SSE true)

if(NOT DEFINED IDK_SSE)
    set(IDK_SSE_FLAGS "-mno-mmx -mno-sse -mno-sse2")
else()
    add_definitions(-DIDK_SSE)
    set(IDK_SSE_FLAGS "-mmmx -msse -msse2")
endif()

# set(IDK_COMMON_FLAGS
#     -ffreestanding -O2 \
#     -Wall \
#     -Wextra \
#     -mgeneral-regs-only \
#     -fno-asynchronous-unwind-tables \
#     -fno-exceptions \
#     -fno-stack-protector \
#     -fno-stack-check \
#     -fno-PIC \
#     -z max-page-size=0x1000 -mno-red-zone \
#     -m64 -mcmodel=kernel -march=x86-64 \
#     -Wno-missing-field-initializers \
# )


set(CMAKE_C_FLAGS
    "-std=gnu11
    -ffreestanding -O2 \
    -Wall \
    -Wextra \
    -mgeneral-regs-only \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -z max-page-size=0x1000 -mno-red-zone \
    -m64 -mcmodel=kernel -march=x86-64 \
    -Wno-missing-field-initializers \
    ${IDK_SSE_FLAGS}"
)


set(CMAKE_CXX_FLAGS
    "-std=c++23 -ffreestanding -O2 \
    -Wall \
    -Wextra \
    -mgeneral-regs-only \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -fno-rtti \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -fpermissive \
    -z max-page-size=0x1000 -mno-red-zone \
    -m64 -mcmodel=kernel -march=x86-64 \
    -MMD \
    -MP \
    -Wno-missing-field-initializers \
    ${IDK_SSE_FLAGS}"

)

set(CMAKE_ASM_NASM_FLAGS
    "-Wall \
    -f elf64"
)

set(
    CMAKE_EXE_LINKER_FLAGS
    "-Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T ${CMAKE_CURRENT_SOURCE_DIR}/system/linker.ld"
)


project(ckos)
ENABLE_LANGUAGE(ASM_NASM)
set(IDK_LIBRARIES "")


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/libc/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/libc++/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/system/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/system/)


file(
    GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/system/ctor/icxxabi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/boot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kmain.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)


# add_library(
#     crti_o OBJECT
#     ${CMAKE_CURRENT_SOURCE_DIR}/system/ctor/crti.asm
# )

# add_library(
#     crtn_o OBJECT
#     ${CMAKE_CURRENT_SOURCE_DIR}/system/ctor/crtn.asm
# )

target_link_libraries(
    ${PROJECT_NAME}

    # crti_o
    # /usr/lib/gcc/x86_64-elf/14.2.0/crtbegin.o

    ck_libc
    ck_libc++
    idk_system

    # /usr/lib/gcc/x86_64-elf/14.2.0/crtend.o
    # crtn_o
)
