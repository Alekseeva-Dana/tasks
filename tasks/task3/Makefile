CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
OBJS = main.o wordlist.o
TARGET = task3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c wordlist.h
	$(CC) $(CFLAGS) -c main.c

wordlist.o: wordlist.c wordlist.h
	$(CC) $(CFLAGS) -c wordlist.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
