set(CMAKE_SYSTEM_NAME "Generic")

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux/bin/x86_64-elf-g++)
# set(CMAKE_C_COMPILER   x86_64-elf-gcc)
# set(CMAKE_CXX_COMPILER x86_64-elf-g++)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/external/x86_64-elf-tools-linux)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)



# set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/linker.ld")
# set(KPROGRAM_LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/src/programs/linker.ld")
# set(SSE_FLAGS "-mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-avx -mgeneral-regs-only")
set(SSE_FLAGS "-mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-avx -mgeneral-regs-only")
# set(SSE_FLAGS "")

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
#     -z max-page-size=0x1000 -mno-red-zone \
#     -mcmodel=kernel \
#     -Wl,--no-relax -Wl,--gc-sections \
#     -Wno-missing-field-initializers"
# )

# https://github.com/dreamportdev/Osdev-Notes/blob/master/01_Build_Process/01_Overview.md

set(CringeOS_C_CXX_FLAGS
    "-O3 \
    ${SSE_FLAGS} \
    -Wall -Wextra -Werror \
    -fstack-protector-strong -fno-strict-aliasing \
    -fno-unwind-tables -fno-asynchronous-unwind-tables \
    -fno-omit-frame-pointer -fno-wrapv \
    -ffreestanding \
    -nostdlib -lgcc \
    -fno-exceptions \
    -fno-rtti \
    -fno-PIC \
    -fno-PIE \
    -Wno-missing-field-initializers \
    \
    -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld \
    -Wl,-m,elf_x86_64 \
    -nostdlib -nodefaultlibs -static -lgcc \
    -mcmodel=kernel \
    -zmax-page-size=0x1000 -mno-red-zone \
    -Wl,--no-relax -Wl,--gc-sections"
)

set(CMAKE_C_FLAGS
    "-std=c11 \
    ${CringeOS_C_CXX_FLAGS}"
)


set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    ${CringeOS_C_CXX_FLAGS} \
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new \
    "
)

set(CMAKE_ASM_NASM_FLAGS
    "-f elf64"
)
