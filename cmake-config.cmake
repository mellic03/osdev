set(KFLAGS_SSE_ON
    -mfpmath=sse -msse -msse2 -msse3 -mno-sse4 -mno-avx -mno-avx2
    -mno-80387 -mno-mmx -mno-3dnow
)

set(KFLAGS_SSE_OFF
    -mgeneral-regs-only
)

set(KERNEL_LDSCRIPT_STATIC "${CMAKE_SOURCE_DIR}/kernel/linker.ld")
set(KERNEL_LDSCRIPT_SHARED "${CMAKE_SOURCE_DIR}/kernel/linker-shared.ld")


set(KFLAGS_COMPILE_COMMON
    -std=c++23
    -include ${CMAKE_SOURCE_DIR}/lib/libc++/include/new
    -O3
    -Wall -Wextra -Werror
    -Wno-missing-field-initializers
    -fsanitize=undefined
    -ftree-vectorize
    -fno-stack-protector
    -fno-unwind-tables -fno-asynchronous-unwind-tables
    -fno-omit-frame-pointer
    -ffreestanding
    -fno-exceptions -fno-rtti
    -m64 -march=x86-64
    -mno-red-zone
)


set(KFLAGS_LINK_KERNEL
    -T ${KERNEL_LDSCRIPT_STATIC}
    -nostdlib -nostdlib++ -nolibc
    -nodefaultlibs -nostartfiles -static
    -lgcc
    -z max-page-size=0x1000
    -Wl,--no-relax -Wl,--gc-sections
)

set(KFLAGS_COMPILE_KERNEL
${KFLAGS_COMPILE_COMMON}
    ${KFLAGS_SSE_OFF}
    -D__is_kernel -mcmodel=kernel -fno-PIC -fno-PIE
    ${KFLAGS_LINK_KERNEL}
)

set(KFLAGS_COMPILE_KERNEL_NOFPU
${KFLAGS_COMPILE_COMMON}
    ${KFLAGS_SSE_OFF}
    -D__is_kernel -mcmodel=kernel -fno-PIC -fno-PIE
    ${KFLAGS_LINK_KERNEL}
)

set(KFLAGS_COMPILE_KERNEL_YESFPU
    ${KFLAGS_COMPILE_COMMON}
    ${KFLAGS_SSE_ON}
    -D__is_kernel -mcmodel=kernel -fno-PIC -fno-PIE
    ${KFLAGS_LINK_KERNEL}
)




# set(KFLAGS_LINK_SHARED
#     -T ${KERNEL_LDSCRIPT_SHARED}
#     -nostdlib -static
#     -lgcc
#     -z max-page-size=0x1000
#     -Wl,--no-relax -Wl,--gc-sections
# )

# set(KFLAGS_COMPILE_SHARED
#     ${KFLAGS_COMPILE_COMMON}
#     -mcmodel=small
#     -fno-PIC -fno-PIE
#     ${KFLAGS_LINK_SHARED}
# )

