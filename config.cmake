set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_NASM_COMPILER nasm)

set(CMAKE_SYSROOT "/home/michael/Desktop/cross")
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# set(CMAKE_C_COMPILER     "${CMAKE_SYSROOT}/bin/x86_64-elf-gcc")
# set(CMAKE_CXX_COMPILER   "${CMAKE_SYSROOT}/bin/x86_64-elf-g++")
# set(CMAKE_INCLUDE_PATH   "${CMAKE_SYSROOT}/lib/gcc/x86_64-elf/16.0.0/include")
# set(CMAKE_CXX_LINK_EXECUTABLE "/home/michael/Desktop/cross/bin/x86_64-elf-g++")

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)


set(KFLAGS_SSE_ON
    -msse -msse2 -msse3 -mno-avx
    -mno-80387 -mno-mmx -mno-3dnow
)

set(KFLAGS_SSE_OFF
    -mgeneral-regs-only
)

# -mfpmath=387 -mhard-float
# -fno-strict-aliasing -fno-wrapv \
# -fno-builtin \

set(KERNEL_LINKER_SCRIPT_STATIC "${CMAKE_SOURCE_DIR}/kernel/linker.ld")
set(KERNEL_LINKER_SCRIPT_SHARED "${CMAKE_SOURCE_DIR}/kernel/linker-shared.ld")


set(KFLAGS_COMPILE_STATIC
    -std=c++23
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new
    -O3
    -Wall -Wextra -Werror
    -Wno-missing-field-initializers
    -ftree-vectorize
    -fno-stack-protector
    -fno-unwind-tables -fno-asynchronous-unwind-tables
    -fno-omit-frame-pointer
    -ffreestanding
    -mno-red-zone
    -fno-exceptions -fno-rtti
    -mcmodel=kernel -m64 -march=x86-64
    -fno-PIC -fno-PIE
    -T ${KERNEL_LINKER_SCRIPT_STATIC}
    -lgcc
    -nostdlib -nostdlib++ -nolibc
    -nodefaultlibs -nostartfiles -static
    -z max-page-size=0x1000
    -Wl,--no-relax -Wl,--gc-sections
)

set(KFLAGS_LINK_STATIC
    -T ${KERNEL_LINKER_SCRIPT_STATIC}
    -lgcc
    -nostdlib -nostdlib++ -nolibc
    -nodefaultlibs -nostartfiles -static
    -z max-page-size=0x1000
    -Wl,--no-relax -Wl,--gc-sections
)


set(KFLAGS_STATIC
   "-std=c++23 \
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new \
    \
    -O3 \
    -Wall -Wextra -Werror \
    -Wno-missing-field-initializers \
    -ftree-vectorize \
    -fno-stack-protector \
    -fno-unwind-tables -fno-asynchronous-unwind-tables \
    -fno-omit-frame-pointer \
    -ffreestanding \
    -mno-red-zone \
    -fno-exceptions -fno-rtti \
    -mcmodel=kernel -m64 -march=x86-64 \
    -fno-PIC -fno-PIE \
    \
    -T ${KERNEL_LINKER_SCRIPT_STATIC} \
    -lgcc \
    -nostdlib -nostdlib++ -nolibc \
    -nodefaultlibs -nostartfiles -static \
    -z max-page-size=0x1000 \
    -Wl,--no-relax -Wl,--gc-sections"
)


# set(GlowOS_GCC_FLAGS
#     "-std=c++23 \
#     -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new \
#     -O3 \
#     ${KFLAGS_SSE_OFF} \
#     -Wall -Wextra -Werror \
#     -Wno-missing-field-initializers \
#     -fsanitize=undefined \
#     -ftree-vectorize \
#     -fno-stack-protector \
#     -fno-unwind-tables -fno-asynchronous-unwind-tables \
#     -fno-omit-frame-pointer \
#     -ffreestanding \
#     -nostdlib -static -nostartfiles \
#     -fno-exceptions \
#     -fno-rtti \
#     -fno-PIC \
#     -fno-PIE \
#     -mno-red-zone \
#     \
#     -T ${CMAKE_SOURCE_DIR}/kernel/linker.ld \
#     -L ${CROSS_LGCC_PATH} \
#     -m64 \
#     -march=x86-64 \
#     -mcmodel=kernel \
#     -z max-page-size=0x1000 \
#     -Wl,--no-relax -Wl,--gc-sections"
# )



# set(KFLAGS_STATIC
#    "-std=c++23 \
#     -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new \
#     \
#     -O3 \
#     ${SSE_FLAGS} \
#     -Wall -Wextra -Werror \
#     -Wno-missing-field-initializers \
#     -fsanitize=undefined \
#     -ftree-vectorize \
#     -fno-stack-protector \
#     -fno-unwind-tables -fno-asynchronous-unwind-tables \
#     -fno-omit-frame-pointer \
#     -ffreestanding \
#     -nostdlib -static -nostartfiles \
#     -fno-exceptions \
#     -fno-rtti \
#     -fno-PIC \
#     -fno-PIE \
#     -mno-red-zone \
#     \
#     -T ${CMAKE_SOURCE_DIR}/kernel/linker-static.ld \
#     -L ${CROSS_LGCC_PATH} \
#     -m64 \
#     -march=x86-64 \
#     -mcmodel=kernel \
#     -z max-page-size=0x1000 \
#     -Wl,--no-relax -Wl,--gc-sections"
# )




# set(KFLAGS_COMPILE_SHARED
#    "-std=c++23 \
#     -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new \
#     -include ${CMAKE_SOURCE_DIR}/external/tinyubsan.hpp \
#     \
#     -O3 \
#     -Wall -Wextra -Werror \
#     -Wno-missing-field-initializers \
#     -fsanitize=undefined \
#     -ftree-vectorize \
#     -fno-stack-protector \
#     -fno-unwind-tables -fno-asynchronous-unwind-tables \
#     -fno-omit-frame-pointer \
#     -ffreestanding -nostdlib -nostdlib++\
#     -fno-exceptions \
#     -fno-rtti \
#     -fPIE -pie \
#     -mno-red-zone \
#     -mcmodel=small -m64 -march=x86-64"
# )

# # -include ${CMAKE_SOURCE_DIR}/kernel/dl_init.cpp \
# set(KFLAGS_LINK_SHARED
#     "-T ${KERNEL_LINKER_SCRIPT_SHARED} \
#     -lgcc -shared \
#     -z max-page-size=0x1000 \
#     -Wl,--no-relax -Wl,--gc-sections"
# )


# SET(CMAKE_CXX_FLAGS ${KFLAGS_STATIC})
set(CMAKE_ASM_NASM_FLAGS "-f elf64")

