CC=gcc
BUILD_DIR=build
CFLAGS= -Wall

BUILD := debug

cflags.common := -std=c11 -Wall 
cflags.debug := -g -O0 
cflags.release := -O2
CFLAGS := ${cflags.${BUILD}} ${cflags.common}

.PHONY: all client_static client_shared_dynamically_linked clean

all:
	make client_static
	make client_shared_so
	make client_shared_dlopen

client_static: client_static.c
	mkdir -p $(BUILD_DIR)
	make -C collatzlib collatz_static BUILD=$(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< collatzlib/build/libcollatz_static.a

client_shared_so: client_shared_so.c
	mkdir -p $(BUILD_DIR)
	make collatz_shared -C collatzlib BUILD=$(BUILD)
	$(CC) $(CFLAGS) -L ./collatzlib/build $< -lcollatz_shared -Wl,-rpath=./collatzlib/build -o $(BUILD_DIR)/$@ 

client_shared_dlopen: client_shared_dlopen.c
	mkdir -p $(BUILD_DIR)
	make collatz_shared -C collatzlib BUILD=$(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -ldl

clean:
	rm -rf $(BUILD_DIR)
	make -C collatzlib clean