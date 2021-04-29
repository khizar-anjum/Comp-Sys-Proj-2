ODIR=obj
SRC = src
INC = include
CC=gcc
FLAGS=-I$(INC) -lpthread

ifeq ($(OS),Windows_NT)
	DelCom=del
else
	DelCom=rm
endif

_OBJ = utils.o Sort.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

directories:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $^ $(FLAGS)

action1: action1.c $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

action2: action2.c $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

action3: action3.c $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

action4: action4.c $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

all: directories action1 action2 action3 action4

.PHONY: clean directories

clean:
	$(DelCom) -rf $(ODIR)
	$(DelCom) action1
	$(DelCom) action2
	$(DelCom) action3
	$(DelCom) action4
