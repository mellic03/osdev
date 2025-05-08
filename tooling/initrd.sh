#!/bin/bash

mkdir -p build

g++ -std=c++23 -o build/initrd initrd.cpp
g++ -std=c++23 -o build/initrd_test initrd_test.cpp


echo "-------- initrd -------- "
./initrd ./test
echo

echo "-------- initrd_test -------- "
./initrd_test
echo

