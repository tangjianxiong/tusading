SOURCE = hash_file_test.c ../../src/hash.c	
OBJ=$(patsubst %.c, %.o, $(SOURCE))	#将.c文件转为.o文件

INCLUDES = -I./hdr	#头文件路径
OPTION=-lcrypto#编译选项

TARGET = hash_file_test
CC = gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(OBJ)  -o output/$(TARGET) $(OPTION)

%.o: %.c 
	$(CC) $(INCLUDE_COMMON) -c $< -o $@  $(OPTION)

.PHONY: clean
clean:
	rm -rf $(OBJ) output/