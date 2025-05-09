
#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'source name required'
    exit 0
fi

NAME=$1

    # -r \

CXX_FLAGS="
    -std=c++23 \
    -I ../include \
    -I ../sysroot/usr/include \
    -include ../sysroot/usr/include/new \
    -include ../sysroot/usr/include/libc++ \
    -mno-sse -mno-sse2 -mno-avx \
    -fno-exceptions \
    -fno-rtti \
    -ffreestanding \
    -nostdlib \
    -fPIE -pie \
    -mcmodel=large \
    -mno-red-zone \
    -z max-page-size=0x1000 \
    -ffunction-sections \
    -T ./linker.ld \
    -MMD \
    -MP"



# DSTDIR=../kernel/initrd/modules
mkdir -p modules
mkdir -p temp
DSTELF=temp/$NAME.elf
DSTBIN=temp/$NAME.bin
COMPILER_EXE=../external/x86_64-elf-tools-linux/bin/x86_64-elf-g++

$COMPILER_EXE -o temp/$NAME.elf ./$NAME.cpp $CXX_FLAGS
mv temp/$NAME.elf modules/$NAME.elf
objcopy -O binary modules/$NAME.elf modules/$NAME.bin

rm dump.txt
echo "-------- readelf -a $NAME.elf --------" >> dump.txt
readelf -a modules/$NAME.elf >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt

echo "-------- objdump -x $NAME.elf --------" >> dump.txt
objdump -x modules/$NAME.elf >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt

echo "-------- objdump -d $NAME.elf --------" >> dump.txt
objdump -d modules/$NAME.elf >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt

echo "-------- hexdump -C $NAME.bin --------" >> dump.txt
hexdump -C modules/$NAME.bin >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt
echo -e "\n" >> dump.txt

rm -rf ./temp


