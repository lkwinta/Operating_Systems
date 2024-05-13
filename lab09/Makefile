# C compiler command
CC=gcc
# defines path to put all binary files
BUILD_DIR=build

# default build configuration, i.e `make all BUILD=release` or `make all BUILD=debug` overwrites this flag 
BUILD := debug

# C flags common in all build configurations
# -Wall - enable all warnings
# -std=c11 - set c standard to 11
cflags.common := -std=c11 -Wall 
# disable all optimizations and add debug information in debug build
cflags.debug := -g -O0 
# enable O2 optimization in release mode
cflags.release := -O2
# build CFLAGS variable based on selected build configuration
CFLAGS := ${cflags.${BUILD}} ${cflags.common}

.PHONY: all clean

# create all executables
all: life

# create executables
life: grid.c life.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) grid.c life.c -o $(BUILD_DIR)/life -lncursesw -lpthread -lm

run_life: life
	./$(BUILD_DIR)/life
	
# removes all unnecesery build objects
clean:
	rm -rf $(BUILD_DIR)