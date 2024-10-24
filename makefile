# Variables
CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I./include
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TARGET = $(BINDIR)/http_server

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Default target to build the program
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create bin and obj directories if they don't exist
$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean target to remove compiled objects and the binary
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Run the server
run: $(TARGET)
	./$(TARGET)

# Run the server with valgrind for memory check
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

.PHONY: all clean run valgrind
