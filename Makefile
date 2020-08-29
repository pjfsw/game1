CC	:= gcc
CCFLAGS:=$(shell sdl-config --cflags) -Wall -O2
LDFLAGS := $(shell sdl-config --libs)
#STATIC := $(shell sdl-config --static-libs)
TARGET := Debug

all:
	mkdir -p $(TARGET)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(TARGET)/game src/*.c 
