#!/bin/bash

mkdir -p build
mkdir -p output

g++ -o build/trig_tables trig_tables.cpp
./build/trig_tables ./output/trig_tables.bin
# ./build/trig_tables ../src/iso_root/data/math_tables.bin

