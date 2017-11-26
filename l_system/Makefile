CC = gcc
CFLAGS = `pkg-config --cflags --libs cairo`

all: main.c lsys.c
	$(CC) -ggdb -o lsys $(CFLAGS) main.c lsys.c 
