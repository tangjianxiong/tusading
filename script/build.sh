#!/bin/bash
#cd ~/tusading
source config
cd kernelspace/
make
mv kernel_k.ko ../out/
make clean
cd ../src
make
make clean
