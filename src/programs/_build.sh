
#!/bin/bash
NAME=$1
OBJDIR="./build"
BINDIR="../../sysroot/bin"

INCLUDES="-I../../sysroot/include"
LIBRARIES="../../sysroot/lib"

CFLAGS="-std=c11 \
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
        -m64 -mcmodel=large -march=x86-64
        -z max-page-size=0x1000 -mno-red-zone \
        -Wno-missing-field-initializers \
        -mmmx -msse -msse2 \
        -nostdlib \
        -ffunction-sections -fdata-sections"

CXXFLAGS="-std=c++23 \
        -O0 \
        $INCLUDES \
        -D__libk_sse \
        -ffreestanding \
        -Wall \
        -Wextra \
        -Werror=return-type \
        -mgeneral-regs-only \
        -fno-asynchronous-unwind-tables \
        -fno-exceptions \
        -fno-stack-protector \
        -fno-stack-check \
        -m64 -mcmodel=large -march=x86-64
        -z max-page-size=0x1000 -mno-red-zone \
        -Wno-missing-field-initializers \
        -mmmx -msse -msse2 \
        -nostdlib \
        -static \
        -ffunction-sections -fdata-sections"

 


mkdir -p $OBJDIR
# mkdir -p $BINDIR

# nasm -felf64 src/_start.asm -o $OBJDIR/_start.o
# # g++ -c  src/$NAME.cpp -o $OBJDIR/$NAME.cpp.o $CXXFLAGS


# g++     -o $NAME.elf $CXXFLAGS \
#         $OBJDIR/_start.o \
#         src/$NAME.cpp \
#         $LIBRARIES/libc.a \
#         $LIBRARIES/libk.a \
#         -Ttext 0xFFFF800100000000 \




nasm -felf64 src/_start.asm -o $OBJDIR/_start.o
g++ -c src/$NAME.cpp -o $OBJDIR/$NAME.cpp.o $CXXFLAGS


ld   -o $OBJDIR/$NAME.elf \
        $OBJDIR/_start.o \
        $OBJDIR/$NAME.cpp.o \
        ../../sysroot/lib/libk.a \
        ../../sysroot/lib/libc.a \
        ../../sysroot/lib/libc++.a \
        -nostdlib \
        -static \
        -z max-page-size=0x1000 \
        -Ttext 0xFFFF800100000000 \
        --no-relax \
        --gc-sections


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
# hexdump -C $BINDIR/$NAME.exec
# echo "---------------------------------------------------------------------"
# echo ""
# echo ""
# echo ""


# rm $OBJDIR/$NAME.elf
# rm $OBJDIR/$NAME.cpp.o

