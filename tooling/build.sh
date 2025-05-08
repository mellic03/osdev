#!/bin/bash

# ./trig_tables.sh
# ./initrd.sh

mkdir -p build
mkdir -p output


CXX_FLAGS="-std=c++23 -g -I../kernel/include/ -I../include/"


# g++ $CXX_FLAGS  -o build/trig_tables     trig_tables.cpp
g++ $CXX_FLAGS  -o ./build/create_ramdisk ./create_ramdisk.cpp

# g++ $CXX_FLAGS -g -o build/read_ramdisk    read_ramdisk.cpp    ../kernel/src/driver/ramfs.cpp
# g++ $CXX_FLAGS  -o build/initrd_test     initrd_test.cpp


# ./build/trig_tables ./output/trig_tables.bin

echo "-------- create_ramdisk -------- "
./build/create_ramdisk ../src/initrd ../src/iso_root/initrd
echo

# echo "-------- read_ramdisk -------- "
# ./build/read_ramdisk ../src/iso_root/initrd
# echo


