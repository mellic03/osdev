# the name of the target operating system
set(CMAKE_SYSTEM_NAME GlowOS)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Compilers
# ----------------------------------------
set(CMAKE_C_COMPILER   /home/michael/Desktop/cross/bin/x86_64-elf-gcc)
set(CMAKE_CXX_COMPILER /home/michael/Desktop/cross/bin/x86_64-elf-g++)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)
set(CMAKE_ASM_NASM_FLAGS "-f elf64")

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
# ----------------------------------------

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /home/michael/Desktop/cross)
# set(CMAKE_FIND_ROOT_PATH  /home/michael/Desktop/code/C++/osdev/external/x86_64-elf-tools-linux)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

