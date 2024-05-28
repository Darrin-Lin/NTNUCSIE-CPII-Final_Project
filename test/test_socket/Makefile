all: sever client

CC = gcc

CFLAGS= -g -Wall -Wextra -fsanitize=address -lubsan -lasan
LDFLAGS= -ggdb3 -O0 --std=c99

sever: sever.c 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	-rm *.o *.d sever client