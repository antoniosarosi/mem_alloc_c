CC=gcc
CFLAGS=-Wall -g
BIN_DIR=./bin
OBJ_DIR=$(BIN_DIR)/objects

main: main.c alloc.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(OBJ_DIR)/alloc.o main.c

alloc.o: alloc.h alloc.c
	mkdir -p $(BIN_DIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/alloc.o -c alloc.c

.PHONY: clean
clean:
	rm -rf bin
