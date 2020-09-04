NAME_A = server_1
NAME_B = client_3


TARGET = $(NAME_A) $(NAME_B)
SOURCE_A =client1.c
SOURCE_B =client3.c
OBJ_A = $(patsubst %.c, %.o, $(SOURCE_A))
OBJ_B = $(patsubst %.c, %.o, $(SOURCE_B))


CC =aarch64-linux-gnu-gcc

all: $(TARGET)
$(NAME_A):$(OBJ_A)
	$(CC) $(OBJ_A) -o out/$(NAME_A) -lpthread

$(NAME_B):$(OBJ_B)
	$(CC) $(OBJ_B) -o out/$(NAME_B) -lpthread


%.o: %.c 
	$(CC) $(INCLUDE_COMMON) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_A) $(OBJ_B) $(OBJ_C) 