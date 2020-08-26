SOURCE = pack_test.c ../../src/encapsulation.c	
OBJ = pack_test.o encapsulation.o	#将.c文件转为.o文件
INCLUDES = -I./hdr	#头文件路径


TARGET = pack_test
CC = aarch64-linux-gnu-gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(OBJ)  -o output/$(TARGET)

encapsulation.o: ../../src/encapsulation.c
	$(CC) $(INCLUDES) -c ../../src/encapsulation.c
pack_test.o: pack_test.c
	$(CC) $(INCLUDES) -c pack_test.c

.PHONY: clean
clean:
	rm -rf $(OBJ) output/