CC = gcc
// CFLAGS = -fsanitize=address -Wall -Wextra -g

build:
	$(CC) -o onegin.out main.cpp $(CFLAGS)

run: build
	./onegin.out