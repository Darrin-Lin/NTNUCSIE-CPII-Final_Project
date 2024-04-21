all: ./test_program

CC = gcc
CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
LDFLAGS= `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -lSDL2_image

./test_program: test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

