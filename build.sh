mkdir -p build

nasm -felf32 src/boot.asm -o build/boot.o
# i686-elf-gpp -c src/kernel.cpp -o build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
i686-elf-g++ -c src/kernel.cpp -o build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

mkdir -p isodir/boot/grub
mkdir -p output
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o output/myos.iso isodir

# qemu-system-i386 -cdrom output/myos.iso
qemu-system-i386 -kernel myos.bin

