CC=gcc
BUILD_DIR=build
CFLAGS= -Wall

BUILD := debug

cflags.common := -std=c11 -Wall
cflags.debug := -g -O0 
cflags.release := -O2
CFLAGS := ${cflags.${BUILD}} ${cflags.common}

.PHONY: all collatz_static collatz_shared clean

all:
	make collatz_static BUILD=$(BUILD)
	make collatz_shared BUILD=$(BUILD)

collatz_static: collatz.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $<
	ar rcs $(BUILD_DIR)/lib$@.a $(BUILD_DIR)/$@

collatz_shared: collatz.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -fPIC -o $(BUILD_DIR)/$@.o -c $<
	$(CC) -shared -o $(BUILD_DIR)/lib$@.so $(BUILD_DIR)/$@.o

clean:
	rm -rf $(BUILD_DIR)