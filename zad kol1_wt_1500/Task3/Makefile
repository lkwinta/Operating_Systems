CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f main.o main

test:  main
	@echo -e "\nTESTING\n"
	./main 6 + 5
	./main 8 x 12
	./main 144 - 137
	./main 72 / 9
	./main d a r
	./main s + t
all: main
