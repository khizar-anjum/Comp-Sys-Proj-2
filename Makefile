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

_OBJ = utils.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

directories:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $^ $(FLAGS)

action1: action1.c $(OBJ)
	$(CC) -o $@ $^ $(FLAGS)

all: directories action1

.PHONY: clean directories

clean:
	$(DelCom) -rf $(ODIR)
	$(DelCom) action1