#!/bin/bash

# qemu-img create -f qcow2 ./output/sysdisk.img 16G

qemu-system-x86_64 \
    -m 4G \
    -smp 4 \
    -cdrom ./output/idkernel.iso \
    -serial stdio \
    -no-reboot -d int,cpu_reset

    # -hdb ./output/sysdisk.img \
