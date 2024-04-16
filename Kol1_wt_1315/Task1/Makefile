CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f main.o main

test:  main
	@echo -e "\n\nTESTING\n\n"
	./main
	./main 2
	./main 1
	./main 9

all: main test
