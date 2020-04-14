#!/bin/sh
mkdir -p build
rm -f ./build/antsim3
echo "-> building..."
cd build
cmake ..
make -j 9
cd ..
echo "-> running..."
./build/antsim3
echo "-> done!"
