NAME_A = client_a
NAME_B = server_a


TARGET = $(NAME_A) $(NAME_B)
SOURCE_A =./client/client.c
SOURCE_B =./server/server.c
OBJ_A = $(patsubst %.c, %.o, $(SOURCE_A))
OBJ_B = $(patsubst %.c, %.o, $(SOURCE_B))


CC =gcc

all: $(TARGET)
$(NAME_A):$(OBJ_A)
	$(CC) $(OBJ_A) -o client/$(NAME_A)

$(NAME_B):$(OBJ_B)
	$(CC) $(OBJ_B) -o server/$(NAME_B)


%.o: %.c 
	$(CC) $(INCLUDE_COMMON) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_A) $(OBJ_B) $(OBJ_C) 