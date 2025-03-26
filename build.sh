#!/bin/bash

mkdir -p ./build
cd ./build
cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain.cmake ../
make -j8
cd ../


cd src/programs
./build.sh # > /dev/null 2>&1
cd ../../


# # git clone https://github.com/limine-bootloader/limine.git \
# #       --branch=v9.x-binary --depth=8 \
# #       external/limine

# cd external
# make -C limine
# cd ../

rm -rf output
mkdir -p output/iso_root/data
mkdir -p output/iso_root/boot/limine

cp -R src/data/* output/iso_root/data/
mv output/iso_root/data/limine.conf output/iso_root/boot/limine/limine.conf

cp -v sysroot/bin/idkernel output/iso_root/boot/
cp -v external/limine/limine-bios.sys \
      external/limine/limine-bios-cd.bin \
      external/limine/limine-uefi-cd.bin \
      output/iso_root/boot/limine/

# Create the EFI boot tree and copy Limine's EFI executables over.
mkdir -p output/iso_root/EFI/BOOT
cp -v external/limine/BOOTX64.EFI output/iso_root/EFI/BOOT/
cp -v external/limine/BOOTIA32.EFI output/iso_root/EFI/BOOT/


# Create ISO.
mkdir -p output
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        output/iso_root -o output/idkernel.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
limine bios-install output/idkernel.iso


