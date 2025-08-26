CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Iinclude $(shell pkg-config --fsanitize=address --cflags libcurl 2>/dev/null)
LDFLAGS := $(shell pkg-config --libs libcurl 2>/dev/null)
# If pkg-config isn't installed, uncomment the next line:
# LDFLAGS := -lcurl

SRC := src/keys.c src/delays.c src/ecp.c src/macros.c src/cli.c src/main.c
OBJ := $(SRC:.c=.o)
BIN := roku-macros

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

