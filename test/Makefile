all: ./test_program ./SDL_test

CC = gcc
CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
LDFLAGS= `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -lSDL2_image -lSDL2_ttf

./test_program: test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

./SDL_test: SDL_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	-rm test.o test_program