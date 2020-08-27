#!/bin/bash
cd ~/tusading
cd kernelspace/
make
mv kernel_k.ko ../out/
make clean
cd ../src
make
make clean
