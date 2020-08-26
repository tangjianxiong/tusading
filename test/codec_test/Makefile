SOURCE = codec_test.c ../../src/codec.c	
OBJ = codec_test.o codec.o	#将.c文件转为.o文件
INCLUDES = -I./hdr	#头文件路径


TARGET = app
CC = gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(OBJ)  -o output/$(TARGET)

codec.o: ../../src/codec.c
	$(CC) $(INCLUDES) -c ../../src/codec.c
codec_test.o: codec_test.c
	$(CC) $(INCLUDES) -c codec_test.c

.PHONY: clean
clean:
	rm -rf $(OBJ) output/