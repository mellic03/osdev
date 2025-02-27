#!/bin/bash

if [[ $# -eq 0 ]] ; then
    echo 'Give me a parameter :D'
    exit 0
fi


# Build project
ARG=$1
SRC_DIR="./"
BUILD_DIR=build/build-$ARG
ISO_PATH=build/iso-$ARG
OUTPUT_PATH=build/ckos-$ARG.iso


mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DKERNEL_ARCH="$1" ../../
make
cd ../../

# Generate ISO file
# mkdir -p $OUTPUT_DIR
# cp $BUILD_DIR/ckos.bin $OUTPUT_DIR/ckos.bin

# mkdir -p isodir/boot/grub
mkdir -p $ISO_PATH
cp -R src/iso_root/. $ISO_PATH
cp $BUILD_DIR/ckos.bin $ISO_PATH/boot/ckos.bin

grub-mkrescue -o $OUTPUT_PATH $ISO_PATH


