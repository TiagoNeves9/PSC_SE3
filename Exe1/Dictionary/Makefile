CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = `pkg-config --cflags --libs glib-2.0`

all: main

main: main.c dictionary.c
	$(CC) $(CFLAGS) -o main $^ $(LIBS)

clean:
	rm -f main

.PHONY: all clean
