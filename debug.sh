#!/bin/bash

# qemu-img create -f qcow2 ./output/sysdisk.img 16G

qemu-system-x86_64 \
    -M q35,accel=tcg\
    -m 4G \
    -smp 4 \
    -cdrom ./output/CringeOS.iso\
    -serial stdio \
    -no-reboot -d int,cpu_reset \
