#!/bin/bash

# qemu-img create -f raw ./output/disk0.img 4G
# qemu-img create -f raw ./output/disk1.img 4G

qemu-system-x86_64 \
    -m 4G \
    -smp 4 \
    -cdrom ./output/CringeOS.iso \
    -serial stdio \
    -enable-kvm \
    -device ati-vga
