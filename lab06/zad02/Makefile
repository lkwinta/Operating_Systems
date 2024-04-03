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
# using direct path to time binary to be able to format time output
TIME := /usr/bin/time

.PHONY: all clean

# create all executables
all: integral_calculator integral_manager
all: integral_calculator integral_manager

# create signal_demo executable
integral_manager: integral_manager.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

integral_calculator: integral_calculator.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

run_calculator: integral_calculator 
	./$(BUILD_DIR)/integral_calculator

run_manager: integral_manager
	./$(BUILD_DIR)/integral_manager
	
# removes all unnecesery build objects
clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.fifo