SOURCE = hash_str_testt.c ../../src/hash.c	
OBJ = hash_str_test.o hash.o	#将.c文件转为.o文件
INCLUDES = -I./hdr	#头文件路径


TARGET = hash_str_test
CC = gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(OBJ)  -o output/$(TARGET)

hash.o: ../../src/hash.c
	$(CC) $(INCLUDES) -c ../../src/hash.c
hash_str_test.o: hash_str_test.c
	$(CC) $(INCLUDES) -c hash_str_test.c

.PHONY: clean
clean:
	rm -rf $(OBJ) output/