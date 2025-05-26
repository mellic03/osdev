


set(CMAKE_SYSTEM_NAME "Generic")

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/bin/x86_64-elf-g++)
# set(CMAKE_C_COMPILER   x86_64-elf-gcc)
# set(CMAKE_CXX_COMPILER x86_64-elf-g++)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)


set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)



# set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/linker.ld")
# set(KPROGRAM_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/programs/linker.ld")
# set(SSE_FLAGS "-mmmx -msse -msse2")
set(SSE_FLAGS "-mno-mmx -mno-sse -mno-sse2")
# add_definitions(-D__libk_sse=true)


# set(CringeOS_C_CXX_FLAGS
#     "-O0 -g \
#     ${SSE_FLAGS} \
#     -fno-inline -fno-tree-vectorize \
#     -Wall -Wextra -Werror \
#     -fsanitize=undefined -fstack-protector-strong -fno-strict-aliasing \
#     -fno-omit-frame-pointer \
#     -ggdb \
#     -fno-exceptions \
#     -fno-asynchronous-unwind-tables \
#     -ffreestanding \
#     -nostdlib \
#     -fno-PIC \
#     -fno-PIE \
#     -mno-80387 \
#     -z max-page-size=0x1000 -mno-red-zone \
#     -mcmodel=kernel \
#     -Wl,--no-relax -Wl,--gc-sections \
#     -Wno-missing-field-initializers"
# )

set(CringeOS_C_CXX_FLAGS
    "-O3 \
    ${SSE_FLAGS} \
    -Wall -Wextra -Werror \
    -fsanitize=undefined -fno-stack-protector -fno-strict-aliasing \
    -fno-omit-frame-pointer \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -ffreestanding \
    -nostdlib \
    -fno-PIC \
    -fno-PIE \
    -mno-80387 \
    -z max-page-size=0x1000 -mno-red-zone \
    -mcmodel=kernel \
    -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld \
    -Wl,--no-relax -Wl,--gc-sections \
    -Wno-missing-field-initializers"
)

set(CMAKE_C_FLAGS
    "-std=c11 \
    ${CringeOS_C_CXX_FLAGS}"
)


set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    ${CringeOS_C_CXX_FLAGS} \
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new \
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/libc++ \
    -fno-rtti"
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
    -mcmodel=kernel \
    -zmax-page-size=0x1000 \
    -mno-red-zone \
    -T${CMAKE_SOURCE_DIR}/kernel/linker.ld"
)


set(
    CMAKE_EXE_LINKER_FLAGS
    "-Wl,-m,elf_x86_64 \
    -Wl,--build-id=none \
    -nostdlib \
    -static \
    -mgeneral-regs-only \
    -z max-page-size=0x1000  -mno-red-zone"
)


# include_directories(
#     ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/include
# )




