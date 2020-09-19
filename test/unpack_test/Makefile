SOURCE = unpack_test.c ../../src/encapsulation.c	
OBJ=$(patsubst %.c, %.o, $(SOURCE))	#将.c文件转为.o文件
INCLUDES = -I./hdr	#头文件路径


TARGET = unpack_test
CC = gcc

$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CC) $(OBJ)  -o output/$(TARGET)
%.o: %.c 
	$(CC) $(INCLUDES) -c $< -o $@  


.PHONY: clean
clean:
	rm -rf $(OBJ) output/