NAME_A = server
NAME_B = client


TARGET = $(NAME_A) $(NAME_B)
SOURCE_A =client1.c md5sum.c ../../src/hash.c 
SOURCE_B =client3.c md5sum.c ../../src/hash.c
OBJ_A = $(patsubst %.c, %.o, $(SOURCE_A))
OBJ_B = $(patsubst %.c, %.o, $(SOURCE_B))

INCLUDE_COMMON = -I./hdr/
OPENSSL=-L/home/tangjianxiong/tmp/tusading/lib/lib -I/home/tangjianxiong/tmp/tusading/lib/include
CC =aarch64-linux-gnu-gcc
OPTION=-lpthread -lssl -lcrypto


all: $(TARGET)
$(NAME_A):$(OBJ_A)
	$(CC) $(OBJ_A) -o out/$(NAME_A)  $(OPENSSL) $(OPTION)

$(NAME_B):$(OBJ_B)
	$(CC) $(OBJ_B) -o out/$(NAME_B)  $(OPENSSL) $(OPTION)


%.o: %.c 
	$(CC) $(INCLUDE_COMMON) -c $< -o $@ $(OPENSSL) $(OPTION)

.PHONY: clean
clean:
	rm -rf $(OBJ_A) $(OBJ_B) $(OBJ_C) 