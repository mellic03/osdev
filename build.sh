#!/bin/bash

mkdir -p ./build
cd ./build


if cmake -DKS_ARCH="i686" ../src/; then
    echo -e "\033[32m ================ Build Successful ================ \033[0m" # Green
    make
else
    echo -e "\033[31m ================ Build Failed ================ \033[0m" # Red
    exit 1
fi

if cmake -DKS_ARCH="x86_64" ../src/; then
    echo -e "\033[32m ================ Build Successful ================ \033[0m" # Green
    make
else
    echo -e "\033[31m ================ Build Failed ================ \033[0m" # Red
    exit 1
fi




cp -r ../src/system/iso_root ./
cp ./bin/* ./iso_root/boot/.

grub-mkrescue -o CoomOS.iso ./iso_root/

