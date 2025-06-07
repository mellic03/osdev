set(CMAKE_SYSTEM_NAME "Generic")

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)

# set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/x86_64-elf-tools-linux/bin/x86_64-elf-gcc)
# set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/x86_64-elf-tools-linux/bin/x86_64-elf-g++)
# set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/x86_64-elf-tools-linux)

set(CMAKE_C_COMPILER   /home/michael/Desktop/cross/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER /home/michael/Desktop/cross/bin/x86_64-elf-g++)
set(CMAKE_FIND_ROOT_PATH /home/michael/Desktop/cross)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# set(SSE_FLAGS "-mfpmath=sse -msse -msse2 -mno-80387 -mno-mmx -mno-avx")
# set(SSE_FLAGS "-mgeneral-regs-only -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-avx")
set(SSE_FLAGS "-mgeneral-regs-only")

# set(CringeOS_COMMON_CXX_FLAGS
#     "-O3 \
#     ${SSE_FLAGS} \
#     -Wall -Wextra -Werror \
#     -Wno-missing-field-initializers \
#     -fsanitize=undefined \
#     -fstack-protector-strong -fno-strict-aliasing \
#     -fno-unwind-tables -fno-asynchronous-unwind-tables \
#     -fno-omit-frame-pointer -fno-wrapv \
#     -ffreestanding \
#     -nostdlib \
#     -fno-exceptions \
#     -fno-rtti \
#     -mno-red-zone \
#     -m64 \
#     -lgcc \
#     -z max-page-size=0x1000 \
#     -Wl,--no-relax -Wl,--gc-sections"
# )
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
# https://github.com/mellic03/osdev/blob/8a5445bfd174a046f8e9d415a3b34d86087c7e20/toolchain.cmake

# set(CringeOS_C_CXX_FLAGS
#     "${CringeOS_COMMON_CXX_FLAGS} \
#     -fno-PIC \
#     -fno-PIE \
#     \
#     -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld \
#     -mcmodel=kernel"
# )

set(CringeOS_C_CXX_FLAGS
    "-O3 \
    ${SSE_FLAGS} \
    -Wall -Wextra -Werror \
    -Wno-missing-field-initializers \
    -fsanitize=undefined \
    -fno-stack-protector -fno-strict-aliasing \
    -fno-unwind-tables -fno-asynchronous-unwind-tables \
    -fno-omit-frame-pointer -fno-wrapv \
    -ffreestanding \
    -nostdlib \
    -fno-exceptions \
    -fno-rtti \
    -fno-PIC \
    -fno-PIE \
    -mno-red-zone \
    \
    -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld \
    -lgcc \
    -mcmodel=kernel \
    -z max-page-size=0x1000 \
    -Wl,--no-relax -Wl,--gc-sections"
)
# -L ${CROSS_LGCC_PATH} \


set(CMAKE_C_FLAGS
    "-std=c11 \
    ${CringeOS_C_CXX_FLAGS}"
)

set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    ${CringeOS_C_CXX_FLAGS} \
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new "
)

set(CMAKE_ASM_NASM_FLAGS
    "-f elf64"
)
