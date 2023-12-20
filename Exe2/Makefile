CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Nome do executável
TARGET = spell_check

# Arquivos fonte
SRCS = spell_check.c

# Objetos gerados durante a compilação
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

