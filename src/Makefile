all: ./engine 
CC = gcc
# CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
CFLAGS = 
LDFLAGS =
TOML_PROGRAM= ./third_party/tomlc99/toml.c ./TOML_process/TOML_get.c
CJSON_PROGRAM= ./third_party/cJSON/cJSON.c ./cJSON_process/save_process.c
DEBUG = #-DDBUG

./engine: engine.c $(TOML_PROGRAM) $(SDL_PROGRAM) $(CJSON_PROGRAM)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(DEBUG)
./test_engine: test.c $(TOML_PROGRAM) $(SDL_PROGRAM) $(CJSON_PROGRAM)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(DEBUG)

.PHONY: clean

clean:
	-rm engine
	-rm test_engine
	# -rm -r ./SDL_process/*.o
	# -rm -r ./TOML_process/*.o
	# -rm -r ./third_party/tomlc99/*.o