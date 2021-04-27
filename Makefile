
CC=gcc

ifeq ($(OS),Windows_NT)
DelCom=del
else
DelCom=rm
endif

all: TeamCreate.o
	$(CC) Action1.c TeamCreate.o -pthread -o Action1.o

TeamCreate.o:
	$(CC) TeamCreate.c -pthread -c -o TeamCreate.o

clean:
	$(DelCom) TeamCreate.o Action1.o