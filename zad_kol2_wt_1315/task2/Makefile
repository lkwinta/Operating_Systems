CFLAGS = -Wall -lpthread
CC = gcc

main: main.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f main.o main

test:  main
	@echo -e "\nTEST\n"
	./main 2 5 4
	./main 0 5 4

all: main
