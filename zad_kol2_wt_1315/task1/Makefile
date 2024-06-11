CFLAGS = -Wall -std=c99 -lpthread
CC = gcc

main:	main.c
	$(CC) $^ -o $@ $(CFLAGS)
clean:
	rm -f main.o main

test:	main
	@echo -e "\n\nTESTING\n\n"
	./main 
all:	main test
