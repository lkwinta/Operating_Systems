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

.PHONY: all tests clean

# create all executables
all: file_sizes

file_sizes: file_sizes.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $< 

gen_tests:
	mkdir -p test_directory

	touch test_directory/file1
	touch test_directory/file2
	touch test_directory/file3
	touch test_directory/file4
	touch test_directory/file5s

	echo "asdaklsdjaklsdjaklsdjlaksdkjsdfkdjngjsdfjkdasnfj" 								>> test_directory/file1
	echo "asdadasdahfghhffhhffghfghfh" 														>> test_directory/file2
	echo "asdajghkjghkjdffgfdhfdghhhhhhhhhhhfhgdshssssssssssssssshsfgwerwrwrewerR" 			>> test_directory/file3
	echo "ASDADAfgsdfgdsgfsdfgsdgdsfgsdgsdgsdgsdfg" 										>> test_directory/file4
	echo "dfsfsdfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffsdfs" 	>> test_directory/file5

tests: gen_tests file_sizes
	./$(BUILD_DIR)/file_sizes test_directory

# removes all unnecesery build objects
clean:
	rm -rf $(BUILD_DIR) test_directory