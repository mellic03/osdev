


set(CMAKE_SYSTEM_NAME "Generic")

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/bin/x86_64-elf-g++)
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
set(SSE_FLAGS "-mmmx -msse -msse2")
# set(SSE_FLAGS "-mno-mmx -mno-sse -mno-sse2")
add_definitions(-D__libk_sse=true)


set(IDKERNEL_C_CXX_FLAGS
    "-O0 \
    -ffreestanding \
    -Wall \
    -Wextra \
    -Werror=return-type \
    -mgeneral-regs-only \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -mno-80387 \
    -z max-page-size=0x1000 -mno-red-zone \
    -m64 -mcmodel=large -march=x86-64 \
    ${SSE_FLAGS} \
    -Wno-missing-field-initializers"
)

set(CMAKE_C_FLAGS
    "-std=c11 \
    ${IDKERNEL_C_CXX_FLAGS}"
)


set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    ${IDKERNEL_C_CXX_FLAGS} \
    -fno-rtti \
    -MMD \
    -MP"
)

set(CMAKE_ASM_NASM_FLAGS
    "-Wall \
    -f elf64"
)

# set(
#     CMAKE_EXE_LINKER_FLAGS
#     "-Wl,-m,elf_x86_64 \
#     -Wl,--build-id=none \
#     -nostdlib \
#     -static \
#     -mgeneral-regs-only \
#     -z max-page-size=0x1000  -mno-red-zone"
# )


include_directories(
    ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/lib/gcc/x86_64-elf/13.2.0/include
)




