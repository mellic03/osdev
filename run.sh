#!/bin/bash

# qemu-img create -f qcow2 ./output/sysdisk.img 16G


qemu-system-x86_64 \
    -enable-kvm \
    -m 4G \
    -smp 4 \
    -cdrom ./output/idkernel.iso \
    -serial stdio \
    -vga qxl \
    # -device virtio-vga,id=display0 \
    # -device virtio-vga,id=display1 \
    # -spice port=5900,disable-ticketing=on

    # -device qxl,id=display1 \
    # -device qxl,id=display2 \
    # -device qxl,id=display3 \
    # \
