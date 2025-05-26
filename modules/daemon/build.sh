
#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'source name required'
    exit 0
fi

NAME=$1
TYPE=$2

if [[ "$TYPE" == "" ]] ; then
    echo RIPERONI
    exit
fi

# -mno-sse -mno-sse2 -mno-avx \

CXX_FLAGS="
    ../kernel/src/tinyubsan.cpp \
    -std=c++23 \
    -I ../include \
    -I ../sysroot/usr/include \
    -I ../sysroot/usr/include/arch/x86_64 \
    -include ../sysroot/usr/include/new \
    -include ../sysroot/usr/include/libc++ \
    -msse -msse2 -mavx \
    -Wall -Wextra -Werror \
    -fstack-protector-strong -fno-strict-aliasing \
    -fno-omit-frame-pointer \
    -fno-asynchronous-unwind-tables \
    -fno-exceptions \
    -ffreestanding \
    -fno-rtti \
    -nostdlib \
    -fPIE -pie \
    -mno-red-zone \
    -mno-80387 \
    -z max-page-size=0x1000 -mno-red-zone \
    -Wl,--no-relax -Wl,--gc-sections \
    -Wno-missing-field-initializers \
    -ffunction-sections \
    -T ./linker.ld \
    -MMD \
    -MP"



mkdir -p ./$TYPE
mkdir -p temp
mkdir -p dump
DSTDIR=driver
DSTELF=temp/$NAME.elf
DSTBIN=temp/$NAME.bin
COMPILER_EXE=../external/x86_64-elf-tools-linux/bin/x86_64-elf-g++


$COMPILER_EXE -o temp/$NAME.elf ./$NAME.cpp $CXX_FLAGS
mv temp/$NAME.elf $TYPE/$NAME.elf
# objcopy -O binary $TYPE/$NAME.elf $TYPE/$NAME.bin


rm dump/$NAME.txt
echo "-------- readelf -a $NAME.elf --------" >> dump/$NAME.txt
readelf -a $TYPE/$NAME.elf >> dump/$NAME.txt
echo -e "\n" >> dump/$NAME.txt
echo -e "\n" >> dump/$NAME.txt
echo -e "\n" >> dump/$NAME.txt

echo "-------- objdump -s $NAME.elf --------" >> dump/$NAME.txt
objdump -d $TYPE/$NAME.elf >> dump/$NAME.txt
echo -e "\n" >> dump/$NAME.txt
echo -e "\n" >> dump/$NAME.txt
echo -e "\n" >> dump/$NAME.txt

# echo "-------- hexdump -C $NAME.bin --------" >> dump.txt
# hexdump -C $TYPE/$NAME.bin >> dump.txt
# echo -e "\n" >> dump.txt
# echo -e "\n" >> dump.txt
# echo -e "\n" >> dump.txt

rm -rf ./temp


