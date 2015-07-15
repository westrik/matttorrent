CC=gcc
CFLAGS= -Wall -pedantic -g -Iinclude
SRC_D=src
OBJ_D=obj
OBJ=main.o
OUT = mwtorrent

.SUFFIXES: .c 
.PHONY: clean

all: $(OUT)

$(OBJ_D):
	mkdir $(OBJ_D)

%.o: $(OBJ_D) $(SRC_D)/%.c
	$(CC) $(CFLAGS) -c $(SRC_D)/$*.c -o $(OBJ_D)/$@

$(OUT) : $(OBJ)
	$(CC) -o $(OUT) $(OBJ_)

clean:
run: all
