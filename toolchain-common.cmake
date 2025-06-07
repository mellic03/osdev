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

set(CringeOS_COMMON_CXX_FLAGS
    "-O3 \
    ${SSE_FLAGS} \
    -Wall -Wextra -Werror \
    -Wno-missing-field-initializers \
    -fsanitize=undefined \
    -fstack-protector-strong -fno-strict-aliasing \
    -fno-unwind-tables -fno-asynchronous-unwind-tables \
    -fno-omit-frame-pointer -fno-wrapv \
    -ffreestanding \
    -nostdlib \
    -fno-exceptions \
    -fno-rtti \
    -mno-red-zone \
    -m64 \
    -lgcc \
    -z max-page-size=0x1000 \
    -Wl,--no-relax -Wl,--gc-sections"
)

# -L ${CROSS_LGCC_PATH} \

