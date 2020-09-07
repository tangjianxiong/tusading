#!/bin/bash
cd kernelk/
make
mv kernel_k.ko ../out/
make clean
cd ..
make
make clean
