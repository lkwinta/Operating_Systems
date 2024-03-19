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
all: spawn_processes

# create execute executable
execute: execute.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -D BYTES

# execute build library with test directory, second call will fail and return error code 2 which will crash make run
test: execute
	mkdir -p test_dir
	touch test_dir/file1
	touch test_dir/file2

	./$(BUILD_DIR)/execute test_dir
	@echo "Return code: $$?"
	./$(BUILD_DIR)/execute asdmasdk
	@echo "Return code: $$?"

# removes all unnecesery build objects
clean:
	rm -rf $(BUILD_DIR) test_dir