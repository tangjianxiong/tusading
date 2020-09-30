#!/bin/bash

###
 # Copyright(c) 2008 - 2021 Allwinner Technology Co. Ltd.
 # All rights reserved.
 # 
 # @CreateDate   : 2020-09-25
 # @Author       : Jianxiong Tang
 # @LastEditTime : 2020-09-30
 # @FilePath     : /tusading/script/build.sh
 # @Description  : the script to compile the project
### 
#cd ~/tusading
source config
cd kernelspace/
make
mv kernel_k.ko ../out/
make clean
cd ../src
make
make clean
