SOURCE = encode_test.c ../../src/codec.c	
OBJ = encode_test.o codec.o	#将.c文件转为.o文件
INCLUDES = -I./hdr	#头文件路径
GCOV=-fprofile-arcs -ftest-coverage

TARGET = encode_test
CC = gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(GCOV) $(OBJ)  -o output/$(TARGET)

codec.o: ../../src/codec.c
	$(CC) $(GCOV) $(INCLUDES) -c ../../src/codec.c
encode_test.o: encode_test.c
	$(CC) $(GCOV) $(INCLUDES) -c encode_test.c

.PHONY: clean
clean:
	rm -rf $(OBJ) output/