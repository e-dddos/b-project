# Define the compiler
CC = gcc

# Define any compile-time flags
CFLAGS = -Wall -Wextra -Iinclude

# Define the target executable
TARGET = app.o

# Define the source directories
SRC_DIR = src
INC_DIR = include

# Define the source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Define the object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to prevent conflicts with files of the same name
.PHONY: all clean
