CC=clang
CFLAGS= -Wall -Wno-unused-function -pedantic -g 

SRC_D=src
OBJ_D=obj

COMMON=                                                                       \
	torrent.o                                                                 \
	bencode.o                                                                 \
	dict.o                                                                    \
	peer.o                                                                    \
	chunk.o
TARGET=                                                                       \
	matttorrent.o
TEST=                                                                         \
	tests.o

COMMON_OUT = $(addprefix $(OBJ_D)/,$(COMMON))
TARGET_OUT = $(addprefix $(OBJ_D)/,$(TARGET))
TEST_OUT = $(addprefix $(OBJ_D)/,$(TEST))
LIB = -L/usr/local/opt/openssl/lib \
		-I/usr/local/opt/openssl/include \
		-lcurl -lssl -lcrypto

INCL = -I/usr/local/opt/openssl/include

BIN = matttorrent
TEST_BIN = test
TESTFILE = data/test.torrent

.SUFFIXES: .c
.PHONY: clean test

all: $(BIN)

$(OBJ_D):
	mkdir $(OBJ_D)

%.o: $(OBJ_D) $(SRC_D)/%.c
	$(CC) $(CFLAGS) $(INCL) -c $(SRC_D)/$*.c -o $(OBJ_D)/$@

$(BIN) : $(COMMON) $(TARGET)
	$(CC) $(LIB) -o $(BIN) $(COMMON_OUT) $(TARGET_OUT) 

clean:
	rm -rf $(OBJ_D)/* $(BIN) $(TEST_BIN)

test: $(COMMON) $(TEST)
	$(CC) -o $(TEST_BIN) $(COMMON_OUT) $(TEST_OUT)
	./$(TEST_BIN)

run: all
	./$(BIN) $(TESTFILE)
