
#!/bin/bash
INCLUDES="- "

CXXFLAGS="-std=c++23 \
        -O0 \
        -I./include \
        -I../libk/include \
        -ffreestanding \
        -Wall \
        -Wextra \
        -Werror=return-type \
        -fno-asynchronous-unwind-tables \
        -fno-exceptions \
        -fno-stack-protector \
        -fno-stack-check \
        -m64 -march=x86-64 \
        -z max-page-size=0x1000 -mno-red-zone \
        -Wno-missing-field-initializers \
        -mno-mmx -mno-sse -mno-sse2 \
        -nostdlib \
        -static \
        -ffunction-sections -fdata-sections"


rm -rf ./build
cp -R ./src ./build

nasm -felf64 build/libk_syscall.asm -o build/libk_syscall.asm.o
find build -name '*.cpp' -exec g++ $CXXFLAGS -c {} -o {}.o \;
g++ -r -o libc.o $(find build -name '*.o')


rm -rf ./build
