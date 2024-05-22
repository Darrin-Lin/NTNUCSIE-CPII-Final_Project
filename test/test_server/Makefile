all: ./test_server

CC = gcc
CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
LDFLAGS= -ggdb3 -O0 --std=c99

./test_server: main.c server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	-rm *.o *.d ./test_server