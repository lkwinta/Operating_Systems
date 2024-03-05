CC=gcc
BUILD_DIR=build
CFLAGS= -Wall

BUILD := debug

cflags.common := -std=c11 -Wall
cflags.debug := -g -O0 
cflags.release := -O2
CFLAGS := ${cflags.${BUILD}} ${cflags.common}

.PHONY: all countdown clean

all:
	make countdown

countdown: countdown.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

clean:
	rm -rf $(BUILD_DIR)