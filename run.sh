#!/bin/bash

# qemu-img create -f qcow2 ./output/sysdisk.img 16G

qemu-system-x86_64 \
    -m 4G \
    -smp 4 \
    -cdrom ./output/ckos.iso \
    -hdb ./output/sysdisk.img \
    -serial stdio \

