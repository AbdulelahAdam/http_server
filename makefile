
CC = gcc
CFLAGS = -Wall -g
SRCDIR = src
OBJDIR = obj
INCDIR = include
BINDIR = bin
TARGET = http_server

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $(BINDIR)/$(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

install: all
	@mkdir -p /usr/local/bin
	cp $(BINDIR)/$(TARGET) /usr/local/bin/
	@echo "Installed $(TARGET) to /usr/local/bin/"

uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled $(TARGET) from /usr/local/bin/"


