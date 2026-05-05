CC = clang
CFLAGS = -std=c23 -Wall -Wextra -Iinclude

SRC_DIR = src
TARGET = program

all:
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
