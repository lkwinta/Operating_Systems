CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f main.o main

test:  main
	@echo -e "\n\nTESTING\n\n"
	./main file1
	./main file1 fsorted
	./main file2 fsorted

all: main test
