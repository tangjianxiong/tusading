#!/bin/bash
mkdir out/
cd kernelk/
make
mv *.ko ../out/
make clean
cd ..
make