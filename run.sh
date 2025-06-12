#!/bin/bash

qemu-img create -f raw ./output/disk0.img 4G
# qemu-img create -f raw ./output/disk1.img 4G

# qemu-system-x86_64 \
#     -m 4G \
#     -smp 4 \
#     -cdrom ./output/CringeOS.iso \
#     -serial stdio


# https://wiki.osdev.org/QEMU

qemu-system-x86_64 \
    -enable-kvm \
    -cpu host \
    -m 4G \
    -smp 4 \
    -drive format=raw,file=output/disk0.img \
    -cdrom ./output/CringeOS.iso \
    -serial stdio \
    -device ati-vga,model=rage128p \
    -display gtk,gl=on
    # -device vmware-svga \
    # -device virtio-gpu-gl-pci \
    # -device virtio-vga-gl \
    # -device virtio-gpu-gl-pci \
