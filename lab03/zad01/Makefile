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
# name and path to raport file
RAPORT := pomiar_zad_2.txt

.PHONY: all tests clean

# create all executables
all: reverse reverse_bytes test_generator

# create reverse executable configured to reverse files byte by byte with BYTES macro
reverse_bytes: reverse.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< -D BYTES

# create reverse executable configured to reverse files in blocks
reverse: reverse.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

# compile random file generator
test_generator: test_generator.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $<

# target performing functionality and performance tests
tests:
	make ftests REVERSE=reverse
	make ftests REVERSE=reverse_bytes

	rm -f pomiar_zad_2.txt
	touch pomiar_zad_2.txt

	make ptests REVERSE=reverse
	@echo >> pomiar_zad_2.txt
	make ptests REVERSE=reverse_bytes

# helper target generating test files with helper generator app
# @ used so commands are not echo'ed while make executed
gen_tests: test_generator
	@ mkdir -p test_files
	@ ./$(BUILD_DIR)/test_generator 512 			test_files/small_file
	@ ./$(BUILD_DIR)/test_generator 15000 			test_files/medium_file
	@ ./$(BUILD_DIR)/test_generator 53248 			test_files/large_file
	@ ./$(BUILD_DIR)/test_generator 10240000 		test_files/huge_file
	@ ./$(BUILD_DIR)/test_generator 102400000 		test_files/monumental_file

# functionality tests, reverses multiple possible files and compares it with result of rev command on the same file
# also @ is used to silence command echoing
# -s in diff means that equality is considered a sucess
ftests: $(REVERSE) gen_tests
	@ mkdir -p test_temp

	@ ./$(BUILD_DIR)/$(REVERSE) test_files/small_file test_temp/small_file_rev
	@ ./$(BUILD_DIR)/$(REVERSE) test_temp/small_file_rev test_temp/small_file_rev_rev

	@ ./$(BUILD_DIR)/$(REVERSE) test_files/medium_file test_temp/medium_file_rev
	@ ./$(BUILD_DIR)/$(REVERSE) test_temp/medium_file_rev test_temp/medium_file_rev_rev

	@ ./$(BUILD_DIR)/$(REVERSE) test_files/large_file test_temp/large_file_rev
	@ ./$(BUILD_DIR)/$(REVERSE) test_temp/large_file_rev test_temp/large_file_rev_rev

	@ ./$(BUILD_DIR)/$(REVERSE) test_files/huge_file test_temp/huge_file_rev
	@ ./$(BUILD_DIR)/$(REVERSE) test_temp/huge_file_rev test_temp/huge_file_rev_rev

	@ diff -s test_files/small_file test_temp/small_file_rev_rev
	@ rev test_files/small_file | diff -s test_temp/small_file_rev -
	@ diff -s test_files/medium_file test_temp/medium_file_rev_rev
	@ rev test_files/medium_file | diff -s test_temp/medium_file_rev -
	@ diff -s test_files/large_file test_temp/large_file_rev_rev
	@ rev test_files/large_file | diff -s test_temp/large_file_rev -
	@ diff -s test_files/huge_file test_temp/huge_file_rev_rev
	@ rev test_files/huge_file | diff -s test_temp/huge_file_rev -

# performance test, creates test file with time that took to reverse given file
# '%e' -  Elapsed real (wall clock) time used by the process, in seconds. -f formats time output
ptests: $(REVERSE) gen_tests
	@ echo "PROGRAM: $(REVERSE)" >> $(RAPORT)
	@ echo "Small file time: " 		`$(TIME) -f '%e' ./$(BUILD_DIR)/$(REVERSE) test_files/small_file 		test_temp/small_file_rev 		2>&1` "[s]" >> $(RAPORT)
	@ echo "Medium file time: " 	`$(TIME) -f '%e' ./$(BUILD_DIR)/$(REVERSE) test_files/medium_file 		test_temp/medium_file_rev 		2>&1` "[s]" >> $(RAPORT)
	@ echo "Large file time: " 		`$(TIME) -f '%e' ./$(BUILD_DIR)/$(REVERSE) test_files/large_file 		test_temp/large_file_rev 		2>&1` "[s]" >> $(RAPORT)
	@ echo "Huge file time: " 		`$(TIME) -f '%e' ./$(BUILD_DIR)/$(REVERSE) test_files/huge_file 		test_temp/huge_file_rev 		2>&1` "[s]" >> $(RAPORT)
	@ echo "Monumental file time: " `$(TIME) -f '%e' ./$(BUILD_DIR)/$(REVERSE) test_files/monumental_file 	test_temp/monumental_file_rev 	2>&1` "[s]" >> $(RAPORT)

# removes all unnecesery build objects
clean:
	rm -rf $(BUILD_DIR) test_files test_temp
	rm -f $(RAPORT)