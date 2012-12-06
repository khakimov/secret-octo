SOURCES=5-6.c
OBJECTS=5-6.o
OPTIONS=-g -Wall -Werror -O0 -std=c99
LINK_FLAGS=-lm
TARGET=56
CC=gcc

$(TARGET): $(OBJECTS)
	$(CC) $(OPTIONS) -o $(TARGET) $(OBJECTS) $(LINK_FLAGS)
$(OBJECTS): $(SOURCES)
	$(CC) -c -g $(SOURCES)
all:
	make $(TARGET)
clean:
	rm -f *.o
	rm -f $(TARGET)
