#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Give me a parameter :D'
    exit 0
fi


# Build project
NAME=$1
ARCH=$2
SRC_DIR="./"
BUILD_DIR=build/$NAME-$ARCH
# ISO_PATH=build/iso-$ARCH
# OUTPUT_PATH=build/ckos-$ARCH.iso


mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKERNEL_ARCH="$ARCH" ../../
make
cd ../../


# mkdir -p $ISO_PATH
# cp -R src/iso_root/. $ISO_PATH
# cp $BUILD_DIR/ckos.bin $ISO_PATH/boot/ckos.bin

# grub-mkrescue -o $OUTPUT_PATH $ISO_PATH


