
#!/bin/bash
NAME=$1
BINDIR="./bin"
STIMDIR="../../lib/libstim/build"
INCLUDES="../../lib/libstim/include"



LD_FLAGS="
    -T ./linker.ld"


C_FLAGS="-std=c11 \
        -O0 \
        -I $INCLUDES \
        -ffreestanding \
        -Wall \
        -Wextra \
        -Werror=return-type \
        -mgeneral-regs-only \
        -fno-asynchronous-unwind-tables \
        -fno-exceptions \
        -fno-stack-protector \
        -fno-stack-check \
        -fno-PIC \
        -mno-80387 \
        -z max-page-size=0x1000 -mno-red-zone \
        -m64 -mcmodel=large -march=x86-64 \
        -Wno-missing-field-initializers \
        -mno-mmx -mno-sse -mno-sse2 \
        -nostdlib"


mkdir -p $BINDIR

gcc -c src/$NAME.c -o src/$NAME.c.o $C_FLAGS

ld  -o src/$NAME.elf \
       src/$NAME.c.o \
    -Ttext 0xFFFF800100000000 \
    $STIMDIR/syscall.asm.o \
    $STIMDIR/file.c.o \
    $STIMDIR/puts.c.o \




# ld --oformat binary -Ttext 0xFFFF800100000000 -o $OBJDIR/$NAME.c.o $OBJDIR/libstim/syscall.asm.o

objcopy -O binary src/$NAME.elf $BINDIR/$NAME.exec

echo ""
echo ""
echo ""
echo ""
echo "-------------------------------objdump-------------------------------"
objdump -d src/$NAME.elf 
echo "---------------------------------------------------------------------"
echo ""
echo ""
echo ""
echo "-------------------------------hexdump-------------------------------"
echo $BINDIR/$NAME.exec
hexdump -C $BINDIR/$NAME.exec
echo "---------------------------------------------------------------------"
echo ""
echo ""
echo ""

