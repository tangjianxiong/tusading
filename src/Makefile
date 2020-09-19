NAME_A = server_a 
NAME_B = client_b
NAME_C = client_c

TARGET = $(NAME_A) $(NAME_B) $(NAME_C)
SOURCE_A =hash.c codec.c encapsulation.c connect.c server_a.c netlink.c 
SOURCE_B =hash.c codec.c encapsulation.c connect.c client_b.c netlink.c 
SOURCE_C =hash.c codec.c encapsulation.c connect.c client_c.c netlink.c 
OBJ_A = $(patsubst %.c, %.o, $(SOURCE_A))
OBJ_B = $(patsubst %.c, %.o, $(SOURCE_B))
OBJ_C = $(patsubst %.c, %.o, $(SOURCE_C))


INCLUDE_COMMON = -I./hdr/
OPENSSL=-L/home/tangjianxiong/tmp/tusading/lib/lib -I/home/tangjianxiong/tmp/tusading/lib/include
CC =aarch64-linux-gnu-gcc
OPTION=-lpthread -lssl -lcrypto

all: $(TARGET)
$(NAME_A):$(OBJ_A)
	$(CC) $(OBJ_A) -o ../out/$(NAME_A) $(OPENSSL) $(OPTION)

$(NAME_B):$(OBJ_B)
	$(CC) $(OBJ_B) -o ../out/$(NAME_B) $(OPENSSL) $(OPTION)

$(NAME_C):$(OBJ_C)
	$(CC) $(OBJ_C) -o ../out/$(NAME_C) $(OPENSSL) $(OPTION)

%.o: %.c 
	$(CC) $(INCLUDE_COMMON) -c $< -o $@ $(OPENSSL) $(OPTION)

.PHONY: clean
clean:
	rm -rf $(OBJ_A) $(OBJ_B) $(OBJ_C) output/