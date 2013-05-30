CC = gcc
OBJ = processor.o
GLIB = `pkg-config --cflags --libs glib-2.0`
processor:$(OBJ)
	$(CC) -o processor $(OBJ) GLIB
processor.o:processor.c
	$(CC) -c processor.c GLIB
