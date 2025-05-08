#!/bin/bash

qemu-img create -f raw ./output/disk0.img 4G
# qemu-img create -f raw ./output/disk1.img 4G

qemu-system-x86_64 \
    -enable-kvm \
    -m 4G \
    -smp 4 \
    -drive file=output/disk0.img,if=ide,format=raw \
    -cdrom ./output/idkernel.iso \
    -serial stdio \
