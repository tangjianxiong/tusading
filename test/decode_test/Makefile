SOURCE = decode_test.c ../../src/codec.c	
OBJ = decode_test.o codec.o	#将.c文件转为.o文件
INCLUDES = -I./hdr	#头文件路径


TARGET = decode_test
CC = gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(OBJ)  -o output/$(TARGET)

codec.o: ../../src/codec.c
	$(CC) $(INCLUDES) -c ../../src/codec.c
decode_test.o: decode_test.c
	$(CC) $(INCLUDES) -c decode_test.c

.PHONY: clean
clean:
	rm -rf $(OBJ) output/