#!/bin/bash

N0="loader"
A0="i686"

N1="kernel"
A1="x86_64"

./build_.sh $N0 $A0
./build_.sh $N1 $A1

cp -R src/iso_root build/

cd build
mv $N0-$A0/$N0.bin iso_root/boot/
mv $N1-$A1/$N1.bin iso_root/boot/


grub-mkrescue -o CoomOS.iso ./iso_root/
