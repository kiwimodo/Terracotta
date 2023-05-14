CC = gcc
CFLAGS = -Wall
DEBUG = -g
LDFLAGS = -shared
INC = -I./include
BUILDDIR = build
SOURCEDIR = src
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)%.c,$(BUILDDIR)%.o,$(SOURCES))
TARGET = libdemo.so

all: library

debug: CFLAGS += $(DEBUG)
debug: library

library: dir $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $<

$(OBJECTS): $(SOURCES)
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)