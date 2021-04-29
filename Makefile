
CC=gcc

ifeq ($(OS),Windows_NT)
DelCom=del
else
DelCom=rm
endif

all: Sort.o
	$(CC) Action2.c Sort.o -pthread -o Action2.o
	$(CC) Action3.c Sort.o -pthread -o Action3.o

Sort.o:
	$(CC) Sort.c -pthread -c -o Sort.o

clean:
	$(DelCom) Sort.o Action2.o Action3.o