obj-m:=kernel_k.o

# KDIR:=~/work/T507/kunos/kernel/linux-4.9/
# ARCH:=arm64 
# CROSS_COMPILEC:=~/work/T507/kunos/out/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

PWD:=$(shell pwd)
default:
	#make -C $(KDIR) M=`pwd` modules
	 make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=`pwd` modules

clean:
	        rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *symvers *Module.markers
