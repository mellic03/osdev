#!/bin/bash

./build.sh

# qemu-system-i386 -cdrom output/myos.iso
qemu-system-i386 -kernel output/CockOS.bin

