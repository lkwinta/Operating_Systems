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
cflags.release := -Ofast
# build CFLAGS variable based on selected build configuration
CFLAGS := ${cflags.${BUILD}} ${cflags.common}
# using direct path to time binary to be able to format time output
TIME := /usr/bin/time

.PHONY: all clean

# create all executables
all: integral

# create signal_demo executable
integral: integral.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -lm

test: integral
	@echo "20 processes	" `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 20 2>&1` "[s]"
	@echo "12 processes " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 12 2>&1` "[s]"
	@echo "5  processes " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 5  2>&1`  "[s]"
	@echo "2  processes " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 2  2>&1`  "[s]"
	@echo "1  process   " `$(TIME) -f '%e' ./$(BUILD_DIR)/integral 0.0000000001 1  2>&1`  "[s]"
	
example: integral
	./$(BUILD_DIR)/integral 0.00001 18
	
# removes all unnecesery build objects
clean:
	rm -rf $(BUILD_DIR)