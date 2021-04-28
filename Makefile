
CC=gcc

ifeq ($(OS),Windows_NT)
DelCom=del
else
DelCom=rm
endif

all: TeamCreate.o Sort.o
	$(CC) Action1.c TeamCreate.o -pthread -o Action1.o
	$(CC) Action2.c TeamCreate.o Sort.o -pthread -o Action2.o

TeamCreate.o:
	$(CC) TeamCreate.c -pthread -c -o TeamCreate.o

Sort.o:
	$(CC) Sort.c -pthread -c -o Sort.o

clean:
	$(DelCom) TeamCreate.o Sort.o Action1.o Action2.o