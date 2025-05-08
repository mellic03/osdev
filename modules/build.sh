
#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'source name required'
    exit 0
fi


NAME=$1



CXX_FLAGS="
    -std=c++23 \
    -I ../sysroot/usr/include \
    -include ../sysroot/usr/include/new \
    -include ../sysroot/usr/include/libc++ \
    -mno-sse -mno-sse2 -mno-avx \
    -fno-exceptions \
    -fno-rtti \
    -ffreestanding \
    -nostdlib \
    -static \
    -fPIC \
    -fPIE \
    -mno-red-zone \
    -z max-page-size=0x1000 \
    -ffunction-sections \
    -T ./linker.ld \
    -MMD \
    -MP"


mkdir -p build
mkdir -p ./$NAME

g++ -o ./$NAME/$NAME.elf ./$NAME.cpp $CXX_FLAGS
# objcopy -O binary ./$NAME/$NAME.o build/$NAME.bin

rm dump.txt
objdump -s ./$NAME/$NAME.elf >> dump.txt
# hexdump -C modules.bin

# rm -rf ./$NAME


