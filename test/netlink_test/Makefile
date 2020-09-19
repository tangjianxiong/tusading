NAME_A = netlink_test
NAME_B = netlink_sendrecv_test

TARGET = $(NAME_A) $(NAME_B)
SOURCE_A = netlink_test.c ../../src/netlink.c	
SOURCE_B = netlink_sendrecv_test.c ../../src/netlink.c	
OBJ_A=$(patsubst %.c, %.o, $(SOURCE_A))	#将.c文件转为.o文件
OBJ_B=$(patsubst %.c, %.o, $(SOURCE_B))

INCLUDES = -I./hdr	-g#头文件路径
CC = aarch64-linux-gnu-gcc

all: $(TARGET)
$(NAME_A): $(OBJ_A)	
	$(CC) $(OBJ_A)  -o out/$(NAME_A) $(INCLUDES)
$(NAME_B): $(OBJ_B)	
	$(CC) $(OBJ_B)  -o out/$(NAME_B) $(INCLUDES)
%.o: %.c 
	$(CC) $(INCLUDES) -c $< -o $@  


.PHONY: clean
clean:
	rm -rf $(OBJ_A) $(OBJ_B) out/