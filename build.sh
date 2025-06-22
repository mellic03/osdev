#!/bin/bash

# Clean up previous build
# ---------------------------------------------------------
rm output/*.iso
rm -rf output/iso_root/*

rm -rf sysroot/drv
rm -rf sysroot/srv
rm -rf sysroot/usr/bin
rm -rf sysroot/usr/lib
rm -rf sysroot/usr/include

mkdir -p output/iso_root/boot/
mkdir -p sysroot/{drv,srv}
mkdir -p sysroot/usr/{bin,lib,include,share}
# ---------------------------------------------------------


# Copy headers to sysroot
# ---------------------------------------------------------
cp -r kernel/include/*     sysroot/usr/include/.
cp -r lib/libc/include/*   sysroot/usr/include/.
cp -r lib/libc++/include/* sysroot/usr/include/.
# ---------------------------------------------------------


# Build modules
# ---------------------------------------------------------
# cd ./modules
# mkdir -p build && cd build
# cmake ..
# make
# cd ../../

# rm modules/mouse.txt
# rm modules/keyboard.txt
# readelf -a sysroot/drv/mouse >> modules/mouse.txt
# readelf -a sysroot/drv/keyboard >> modules/keyboard.txt
# ---------------------------------------------------------


# Compile kernel
# ---------------------------------------------------------
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=cmake-toolchain.cmake ..
make -j8
cd ../
rm dump.txt
# readelf -a sysroot/usr/bin/CringeOS >> dump.txt
objdump -d sysroot/usr/bin/CringeOS >> dump.txt
# readelf -a sysroot/usr/lib/libc.a >> dump.txt
# objdump -d sysroot/usr/lib/libc.a >> dump.txt
# hexdump -C sysroot/usr/lib/libc.a >> dump.txt
# ---------------------------------------------------------


# Create initrd tarball and move to output folder
# ---------------------------------------------------------
cd ./sysroot
tar -cvf initrd.tar.gz * > /dev/null 2>&1
mv ./initrd.tar.gz ../output/iso_root/initrd.tar.gz
cd ../
# ---------------------------------------------------------


# cp sysroot/bin/*.elf src/programs/build/
# cd src/programs
# ./build.sh >> /dev/null 2>&1
# cd ../../
# cp src/programs/bin/* src/iso_root/bin

# git clone https://github.com/limine-bootloader/limine.git \
#       --branch=v9.x-binary --depth=8 \
#       external/limine
# cd external
# make -C limine
# cd ../


mkdir -p output/iso_root/boot/
cp -R kernel/limine.conf output/iso_root/boot/limine.conf

cp -v sysroot/usr/bin/CringeOS output/iso_root/
cp -v external/limine/limine-bios.sys \
      external/limine/limine-bios-cd.bin \
      external/limine/limine-uefi-cd.bin \
      output/iso_root/boot/

# Create the EFI boot tree and copy Limine's EFI executables over.
mkdir -p output/iso_root/EFI/BOOT
cp -v external/limine/BOOTX64.EFI output/iso_root/EFI/BOOT/
cp -v external/limine/BOOTIA32.EFI output/iso_root/EFI/BOOT/


# Create ISO.
mkdir -p output
xorriso -as mkisofs -R -r -J -b boot/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        output/iso_root -o output/CringeOS.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
limine bios-install output/CringeOS.iso

