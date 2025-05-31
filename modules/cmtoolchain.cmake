set(CMAKE_SYSTEM_NAME "Generic")

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# set(CMAKE_C_COMPILER   ${CMAKE_SOURCE_DIR}/../external/x86_64-elf-tools-linux/bin/x86_64-elf-gcc)
# set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/../external/x86_64-elf-tools-linux/bin/x86_64-elf-g++)
set(CMAKE_C_COMPILER   /home/michael/Desktop/cross/bin/x86_64-elf-gc)
set(CMAKE_CXX_COMPILER /home/michael/Desktop/cross/bin/x86_64-elf-g++)

set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
# set(CMAKE_FIND_ROOT_PATH ${CMAKE_SOURCE_DIR}/../external/x86_64-elf-tools-linux)
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(SSE_FLAGS "-mno-80387 -mno-mmx -mno-sse -mno-avx -mgeneral-regs-only")

set(CringeModules_C_CXX_FLAGS
    "-O3 ${SSE_FLAGS} \
    -Wall -Wextra -Werror \
    -fno-strict-aliasing \
    -fno-omit-frame-pointer \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -ffreestanding \
    -fno-rtti \
    -nostdlib \
    -fPIE -pie \
    -z max-page-size=0x1000 -mno-red-zone \
    -T ${CMAKE_SOURCE_DIR}/linker.ld \
    -Wl,--no-relax -Wl,--gc-sections \
    -Wno-missing-field-initializers \
    -ffunction-sections"
)

set(CMAKE_C_FLAGS
    "-std=c11 \
    ${CringeModules_C_CXX_FLAGS}"
)

set(CMAKE_CXX_FLAGS
    "-std=c++23 \
    ${CringeModules_C_CXX_FLAGS} \
    -include ${CMAKE_SOURCE_DIR}/../lib/libc++/include/new \
    -fno-rtti"
)

set(CMAKE_ASM_NASM_FLAGS
    "-Wall \
    -f elf64"
)
