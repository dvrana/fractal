CC = gcc
CFLAGS = `pkg-config --cflags --libs cairo`

all: main.c diamond_square.c
	$(CC) -ggdb -o ds $(CFLAGS) main.c diamond_square.c 
