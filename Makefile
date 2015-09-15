CC=gcc
CFLAGS= -Wall -pedantic -g -Iinclude

SRC_D=src
OBJ_D=obj

OBJ=bencode.o main.o
OBJ_OUT = $(addprefix $(OBJ_D)/,$(OBJ))

BIN = matttorrent
TESTFILE = test/test.torrent

.SUFFIXES: .c 
.PHONY: clean

all: $(BIN)

$(OBJ_D):
	mkdir $(OBJ_D)

%.o: $(OBJ_D) $(SRC_D)/%.c
	$(CC) $(CFLAGS) -c $(SRC_D)/$*.c -o $(OBJ_D)/$@

$(BIN) : $(OBJ)
	$(CC) -o $(BIN) $(OBJ_OUT)

clean:
	rm -rf $(OBJ_OUT) $(BIN)

run: all
	./mwtorrent $(TESTFILE)
