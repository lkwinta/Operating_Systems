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

# create all client executables
all: reverse test_generator

reverse: reverse.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<
	
test_generator: test_generator.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

# example program execution
test_reverse:
	./$(BUILD_DIR)/reverse test reversed_test
	./$(BUILD_DIR)/reverse reversed_test reversed_test_reversed
	diff -s test reversed_test_reversed

PASSED = 0
functionality_tests: all
	@ mkdir -p test_files
	@ mkdir -p test_temp

	@ ./$(BUILD_DIR)/test_generator 512 		test_files/small_file
	@ ./$(BUILD_DIR)/test_generator 1500 		test_files/medium_file
	@ ./$(BUILD_DIR)/test_generator 5234 		test_files/large_file
	@ ./$(BUILD_DIR)/test_generator 102400 		test_files/huge_file

	@ ./$(BUILD_DIR)/reverse test_files/small_file test_temp/small_file_rev
	@ ./$(BUILD_DIR)/reverse test_temp/small_file_rev test_temp/small_file_rev_rev

	@ ./$(BUILD_DIR)/reverse test_files/medium_file test_temp/medium_file_rev
	@ ./$(BUILD_DIR)/reverse test_temp/medium_file_rev test_temp/medium_file_rev_rev

	@ ./$(BUILD_DIR)/reverse test_files/large_file test_temp/large_file_rev
	@ ./$(BUILD_DIR)/reverse test_temp/large_file_rev test_temp/large_file_rev_rev

	@ ./$(BUILD_DIR)/reverse test_files/huge_file test_temp/huge_file_rev
	@ ./$(BUILD_DIR)/reverse test_temp/huge_file_rev test_temp/huge_file_rev_rev

	@ diff -s test_files/small_file test_temp/small_file_rev_rev
	@ rev test_files/small_file | diff -s test_temp/small_file_rev -
	@ diff -s test_files/medium_file test_temp/medium_file_rev_rev
	@ rev test_files/medium_file | diff -s test_temp/medium_file_rev -
	@ diff -s test_files/large_file test_temp/large_file_rev_rev
	@ rev test_files/large_file | diff -s test_temp/large_file_rev -
	@ diff -s test_files/huge_file test_temp/huge_file_rev_rev
	@ rev test_files/huge_file | diff -s test_temp/huge_file_rev -

clean:
	rm -rf $(BUILD_DIR) test_files test_temp