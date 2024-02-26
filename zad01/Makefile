CC=gcc
CFLAGS=-Wall

build_link_shared_debug:
	make -C libwc lib_shared
	$(CC) $(CFLAGS) -L ./libwc/build -Wall main.c -lwc -o main
	export $LD_LIBRARY_PATH=`pwd`/libwc/build
