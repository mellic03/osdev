#!/bin/bash

# ./build.sh

qemu-system-x86_64 \
    -m 1G \
    -smp 4 \
    -cdrom ./output/ckos.iso \
    -serial stdio \




