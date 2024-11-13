# Compiler und Flags
CC = gcc
CFLAGS = -Wall -std=c11

# Target-Executable
TARGET = myfind

# Code-Files
SRC = main.c myfind.c

# Standard-Target
all: $(TARGET)

# Create Executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean
clean:
	rm -f $(TARGET)
