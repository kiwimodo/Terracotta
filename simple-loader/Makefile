ifeq (,$(filter clean,$(MAKECMDGOALS)))
ifeq (,$(wildcard $(SO_PATH)))
    $(error SO_PATH is not defined!)
endif
endif
VAR=$(shell echo $(SO_PATH) | sed 's/\//_/g' | sed 's/\./_/g')

CC = gcc
CFLAGS = -Wall
DEBUG = -g
BUILDDIR = build
SOURCEDIR = src
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)%.c,$(BUILDDIR)%.o,$(SOURCES))
BLOB = blob.h
TARGET = loader

all: library

debug: CFLAGS += $(DEBUG)
debug: library

library: dir $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) -o $@ $< -lrt -ldl

$(OBJECTS): $(SOURCES) $(BUILDDIR)/$(BLOB)
	$(CC) -c $(CFLAGS) $(MACRO) -I./$(BUILDDIR) $< -o $@

$(BUILDDIR)/$(BLOB):
	echo "#ifndef blob_h__\n#define blob_h__\n\n" >> $@
	xxd -include $(SO_PATH) >> $@
	sed -i 's/$(VAR)/blob/g' $@
	sed -i 's/unsigned/static const unsigned/g' $@
	echo "#endif" >> $@

dir:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
