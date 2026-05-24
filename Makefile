CC = clang
CFLAGS = -std=c23 -Wall -Wextra -Iinclude

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
TARGET = program

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
