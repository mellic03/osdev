
# Compile boot.asm manually since CMake is being cringe about it
nasm -felf32 src/kernel/boot.asm -o src/kernel/boot.o


# Build project
mkdir -p build
cd build
cmake ../
make
cd ../


# Delete compiled boot.o
rm src/kernel/boot.o



# Generate ISO file
mkdir -p output
cp build/CockOS.bin output/CockOS.bin

mkdir -p isodir/boot/grub
cp output/CockOS.bin isodir/boot/CockOS.bin
cp src/kernel/grub.cfg isodir/boot/grub/grub.cfg

grub-mkrescue -o output/CockOS.iso isodir
rm -rf isodir



