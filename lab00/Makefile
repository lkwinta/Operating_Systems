CC=gcc
BUILD=build
CFLAGS= -g -Wall

.PHONY: all countdown clean

all:
	make countdown

countdown: countdown.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/$@ $<

clean:
	rm -rf $(BUILD)