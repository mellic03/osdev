set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(CROSS_LGCC_PATH      /home/michael/Desktop/cross/lib/gcc/x86_64-elf/16.0.0/libgcc.a)
set(CMAKE_C_COMPILER     /home/michael/Desktop/cross/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER   /home/michael/Desktop/cross/bin/x86_64-elf-g++)
set(CMAKE_LINKER         /home/michael/Desktop/cross/bin/x86_64-elf-gcc)

# set(SSE_FLAGS "-mfpmath=387 -mhard-float -m80387 -mmmx -msse -msse2 -mavx")
set(SSE_FLAGS "-mno-80387 -mno-mmx -msse -msse2 -mno-avx")
# -fno-strict-aliasing -fno-wrapv \
# -fno-builtin \

set(GlowOS_GCC_FLAGS
    "-O3 \
    ${SSE_FLAGS} \
    -Wall -Wextra -Werror \
    -Wno-missing-field-initializers \
    -ftree-vectorize \
    -fno-stack-protector \
    -fno-unwind-tables -fno-asynchronous-unwind-tables \
    -fno-omit-frame-pointer \
    -ffreestanding \
    -nostdlib -static -nostartfiles \
    -fno-exceptions \
    -fno-rtti \
    -fno-PIC \
    -fno-PIE \
    -mno-red-zone \
    \
    -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld \
    -L ${CROSS_LGCC_PATH} \
    -m64 \
    -march=x86-64 \
    -mcmodel=kernel \
    -z max-page-size=0x1000 \
    -Wl,--no-relax -Wl,--gc-sections"
)


SET(CMAKE_C_FLAGS "-std=c11 ${GlowOS_GCC_FLAGS}" CACHE STRING "" FORCE)
SET(CMAKE_CXX_FLAGS "-std=c++23 ${GlowOS_GCC_FLAGS} -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new" CACHE STRING "" FORCE)
set(CMAKE_ASM_NASM_FLAGS "-f elf64" CACHE STRING "" FORCE)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)