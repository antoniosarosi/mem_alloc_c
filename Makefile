CC=gcc
CFLAGS=-Wall -g
SRC_DIR=./src
BIN_DIR=./bin
OBJ_DIR=$(BIN_DIR)/objects

main: $(SRC_DIR)/main.c $(OBJ_DIR)/alloc.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(OBJ_DIR)/alloc.o $(SRC_DIR)/main.c

$(OBJ_DIR)/alloc.o: $(SRC_DIR)/alloc.h $(SRC_DIR)/alloc.c
	mkdir -p $(BIN_DIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/alloc.o -c $(SRC_DIR)/alloc.c

.PHONY: clean
clean:
	rm -rf bin
