#!/bin/bash


mkdir -p .temp

cp -R lib .temp/
cp -R src .temp/
cp makefile .temp/makefile

cd .temp
make NAME=program

mv program.exec ../bin

cd ../
rm -rf .temp








# objcopy -O binary -j .text $OBJDIR/$NAME.elf $BINDIR/$NAME.bin

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
# hexdump -C $BINDIR/$NAME.bin
# echo "---------------------------------------------------------------------"
# echo ""
# echo ""
# echo ""





