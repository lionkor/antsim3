#!/bin/sh
mkdir -p build
rm -f ./build/antsim3
cd build
cmake .. 
cd ..
make -j 9 -C build && ./build/$*
