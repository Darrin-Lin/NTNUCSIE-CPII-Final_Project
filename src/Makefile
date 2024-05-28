all: ./engine
CC = gcc
CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
LDFLAGS= `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -lSDL2_image -lSDL2_ttf
TOML_PROGRAM= ./third_party/tomlc99/toml.c ./toml_process/toml_get.c
SDL_PROGRAM= ./SDL_process/window_process.c
DEBUG = #-DDBUG

./engine: engine.c $(TOML_PROGRAM) $(SDL_PROGRAM)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(DEBUG)

.PHONY: clean

clean:
	-rm engine