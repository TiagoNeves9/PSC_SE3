CC = gcc
CFLAGS = -Wall -Wextra -g
LIBS = `pkg-config --cflags --libs glib-2.0`
LIB_NAME = libdictionary

all: static dynamic

static: dictionary_static

dynamic: dictionary_dynamic

dictionary_static: dictionary.c
	$(CC) $(CFLAGS) -c -o dictionary.o dictionary.c $(LIBS)
	ar rcs $(LIB_NAME).a dictionary.o

dictionary_dynamic: dictionary.c
	$(CC) $(CFLAGS) -fPIC -c -o dictionary.o dictionary.c $(LIBS)
	$(CC) -shared -o $(LIB_NAME).so dictionary.o

clean:
	rm -f dictionary.o $(LIB_NAME).a $(LIB_NAME).so mainStatic mainDinamic
