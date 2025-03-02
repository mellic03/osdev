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

set(CMAKE_C_FLAGS
    "-std=gnu11 -ffreestanding -O2 -DLIMINE_API_REVISION=3 \
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
    -mno-mmx -mno-sse -mno-sse2 \
    -Wno-missing-field-initializers"
)

set(CMAKE_CXX_FLAGS
    "-std=c++17 -ffreestanding -O2 -DLIMINE_API_REVISION=3 \
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
    -mno-mmx -mno-sse -mno-sse2 \
    -MMD \
    -MP \
    -Wno-missing-field-initializers"
)

set(CMAKE_ASM_NASM_FLAGS
    "-Wall \
    -f elf64"
)

set(
    CMAKE_EXE_LINKER_FLAGS
    "-Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -ffreestanding \
    -nostdlib \
    -m64 -mcmodel=kernel -march=x86-64 \
    -z max-page-size=0x1000 \
    -T ${CMAKE_SOURCE_DIR}/system/linker.ld"
)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")



project(ckos)
ENABLE_LANGUAGE(ASM_NASM)




include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/libc/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/libc++/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/system/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)


add_subdirectory(${CMAKE_SOURCE_DIR}/lib/)
add_subdirectory(${CMAKE_SOURCE_DIR}/system/ck_systemio/)
add_subdirectory(${CMAKE_SOURCE_DIR}/system/drivers/)
add_subdirectory(${CMAKE_SOURCE_DIR}/system/interrupt2/)

# add_subdirectory(${CMAKE_SOURCE_DIR}/system/loader/)
# add_subdirectory(${CMAKE_SOURCES_DIR}/system/kernel/)

file(
    GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/system/kmain.cpp
    ${CMAKE_SOURCE_DIR}/system/boot/boot.cpp
    ${CMAKE_SOURCE_DIR}/system/io.cpp
    ${CMAKE_SOURCE_DIR}/system/*.cpp


    # ${CMAKE_SOURCE_DIR}/system/loader/gdt-32.cpp
    # ${CMAKE_SOURCE_DIR}/system/gdt.cpp
    # ${CMAKE_SOURCE_DIR}/system/gdt.s
    # ${CMAKE_SOURCE_DIR}/system/idt.cpp
    # ${CMAKE_SOURCE_DIR}/system/idt.s
    # ${CMAKE_SOURCE_DIR}/test/gdt.c
    # ${CMAKE_SOURCE_DIR}/test/gdt.s

)


add_executable(
    ${PROJECT_NAME}
    ${SOURCES}

    # ${CMAKE_SOURCE_DIR}/system/kcommon/ctor/icxxabi.cpp
    # ${CMAKE_SOURCE_DIR}/system/drivers/serial.cpp
    # ${CMAKE_SOURCE_DIR}/system/drivers/ps2.cpp
)



target_link_libraries(
    ${PROJECT_NAME}

    # ${CMAKE_SOURCE_DIR}/system/kcommon/ctor/crti.o
    # /usr/lib/gcc/x86_64-elf/14.2.0/crtbegin.o

    ck_libc
    ck_libc++
    ck_systemio
    ck_drivers
    cpu_interrupts
    # /usr/lib/gcc/x86_64-elf/14.2.0/crtend.o
    # ${CMAKE_SOURCE_DIR}/system/kcommon/ctor/crtn.o
)
