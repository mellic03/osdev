#!/bin/bash
mkdir -p ./build
cd ./build
# cmake -G "Ninja" ../src/
# ninja -j8
cmake ../src/
make -j4
cd ../


# git clone https://github.com/limine-bootloader/limine.git \
#       --branch=v9.x-binary --depth=8 \
#       ./external/limine

# cd external
# make -C limine
# cd ../
mkdir -p ./iso_root/data
mkdir -p ./iso_root/boot/limine


cp ./src/data/* ./iso_root/data/
mv ./iso_root/data/limine.conf ./iso_root/boot/limine/limine.conf

cp -v ./build/ckos ./iso_root/boot/
cp -v ./external/limine/limine-bios.sys \
      ./external/limine/limine-bios-cd.bin \
      ./external/limine/limine-uefi-cd.bin \
      ./iso_root/boot/limine/

# Create the EFI boot tree and copy Limine's EFI executables over.
mkdir -p ./iso_root/EFI/BOOT
cp -v ./external/limine/BOOTX64.EFI ./iso_root/EFI/BOOT/
cp -v ./external/limine/BOOTIA32.EFI ./iso_root/EFI/BOOT/


# Create ISO.

mkdir -p ./output
xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        iso_root -o ./output/ckos.iso

# Install Limine stage 1 and 2 for legacy BIOS boot.
./external/limine/limine bios-install ./output/ckos.iso


