
#!/bin/bash
INCLUDES="./include"

C_FLAGS="-std=c11 \
        -O0 \
        -I $INCLUDES \
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
        -Wno-missing-field-initializers \
        -mno-mmx -mno-sse -mno-sse2 \
        -nostdlib"


 
rm -rf ./build
mkdir -p ./build

nasm -felf64 src/syscall.asm -o build/syscall.asm.o
gcc  -c      src/file.c      -o build/file.c.o $C_FLAGS
gcc  -c      src/puts.c      -o build/puts.c.o $C_FLAGS



