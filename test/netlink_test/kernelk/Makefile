obj-m:=kernel_test.o
KDIR:=~/work/T507/kunos/kernel/linux-4.9/
#/home/xufang/workspace/T5anQ/kunos/out/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

PWD:=$(shell pwd)
default:
	make ARCH=arm64 CROSS_COMPILE=~/work/T507/kunos/out/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -C $(KDIR) M=`pwd` modules

clean:
	        rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *symvers *Module.markers
