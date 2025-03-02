#!/bin/bash

# ./build.sh

qemu-system-x86_64 \
    -m 1G \
    -smp 4 \
    -cdrom ./output/ckos.iso \
    -serial stdio \


# qemu-system-x86_64 -m 4G -cdrom  ./image.iso -serial stdio
# qemu-system-i386 -m 4G -cdrom ./build/CoomOS.iso -serial stdio
# qemu-system-x86_64 -m 4G -cdrom build/CoomOS.iso -serial file:serial.txt


