CC=gcc
CFLAGS=-Wall
LIB=libwc
BUILD=build

all:
	make lib_static
	make lib_shared

lib_static: $(LIB).c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $(LIB).c -o $(BUILD)/$(LIB).o
	ar cr $(BUILD)/$(LIB).a $(BUILD)/$(LIB).o

lib_shared: $(LIB).c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c -fPIC $(LIB).c -o $(BUILD)/$(LIB).o
	$(CC) $(CFLAGS) -shared -o $(BUILD)/$(LIB).so $(BUILD)/$(LIB).o
	cp libwc_so.h $(BUILD)/libwc_so.h

clean:
	rm -rf build
	rm -f *.o *.gch *.a *.so