CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = `pkg-config --cflags --libs glib-2.0`

all: main obj

main: main.c dictionary.c
	$(CC) $(CFLAGS) -o main.o $^ $(LIBS)

clean:
	rm -f main.o

.PHONY: all clean
