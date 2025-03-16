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
    "-std=c11 \
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
    -mmmx -msse -msse2 \
    -Wno-missing-field-initializers"
)


set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    -ffreestanding -O2 \
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
    -mmmx -msse -msse2 \
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
    -nostdlib \
    -static \
    -z max-page-size=0x1000 \
    -T ${CMAKE_SOURCE_DIR}/linker.ld"
)

project(idkernel)
ENABLE_LANGUAGE(ASM_NASM)


set(IDKERNEL_LIBRARIES "")
set(IDKERNEL_SOURCES "")


include_directories("${CMAKE_SOURCE_DIR}/external/")
include_directories("${CMAKE_SOURCE_DIR}/libkernel/include/")
include_directories("${CMAKE_SOURCE_DIR}/libc/include/")
include_directories("${CMAKE_SOURCE_DIR}/libc++/include/")
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_subdirectory("${CMAKE_SOURCE_DIR}/libc/")
add_subdirectory("${CMAKE_SOURCE_DIR}/libc++/")
add_subdirectory("${CMAKE_SOURCE_DIR}/libkernel/")
add_subdirectory("${CMAKE_SOURCE_DIR}/kernel/")


add_executable(
    ${PROJECT_NAME}
    ${IDKERNEL_SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}
    idk_libc
    idk_libc++
    ${IDKERNEL_LIBRARIES}
)

