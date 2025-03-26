
#!/bin/bash
NAME=$1
OBJDIR="./build"
BINDIR="../../sysroot/bin"

INCLUDES="-I../../sysroot/include"
LIBRARIES="../../sysroot/lib"

CXXFLAGS="-std=c++23 \
        -D__libk_sse \
        -O0 \
        $INCLUDES \
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
        -mmmx -msse -msse2 \
        -Wno-missing-field-initializers"

 


mkdir -p $OBJDIR
# mkdir -p $BINDIR

# nasm -felf64 src/_start.asm -o $OBJDIR/_start.o
# g++ -c  src/$NAME.cpp -o $OBJDIR/$NAME.cpp.o $CXXFLAGS


g++  -o $OBJDIR/$NAME.elf \
        $OBJDIR/_start.o \
        src/$NAME.cpp \
        $CXXFLAGS \
        ../../sysroot/lib/libk.a \
        ../../sysroot/lib/libc.a \
        ../../sysroot/lib/libc++.a \
        -nostdlib \
        -static \
        -z max-page-size=0x1000 \
        -T ./linker.ld \
        -Wl,--no-relax \
        -Wl,--gc-sections



# nasm -felf64 src/_start.asm -o $OBJDIR/_start.o
# g++ -c src/$NAME.cpp -o $OBJDIR/$NAME.cpp.o $CXXFLAGS


# ld   -o $OBJDIR/$NAME.elf \
#         $OBJDIR/_start.o \
#         $OBJDIR/$NAME.cpp.o \
#         ../../sysroot/lib/libk.a \
#         ../../sysroot/lib/libc.a \
#         ../../sysroot/lib/libc++.a \
#         -nostdlib \
#         -static \
#         -z max-page-size=0x1000 \
#         -Ttext 0xFFFF800100000000 \
#         --no-relax \
#         --gc-sections

#         # -T ./linker.ld \


objcopy -O binary $OBJDIR/$NAME.elf ../data/exec/$NAME.exec



# echo ""
# echo ""
# echo ""
# echo ""
# echo "-------------------------------objdump-------------------------------"
# objdump -d $OBJDIR/$NAME.elf
# echo "---------------------------------------------------------------------"
# echo ""
# echo ""
# echo ""
# echo "-------------------------------hexdump-------------------------------"
# echo $BINDIR/$NAME.exec
# hexdump -C ../data/exec/$NAME.exec
# echo "---------------------------------------------------------------------"
# echo ""
# echo ""
# echo ""


# rm $OBJDIR/$NAME.elf
# rm $OBJDIR/$NAME.cpp.o

