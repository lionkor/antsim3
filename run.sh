#!/bin/sh
mkdir -p build
rm -f ./build/antsim3
echo "-> building..."
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 9
cd ..
echo "-> running..."
./build/antsim3
echo "-> done!"
