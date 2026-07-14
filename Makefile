CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
SRC = src/main.c src/container.c src/cgroups.c

all: minictl

minictl: $(SRC)
	$(CC) $(CFLAGS) -o minictl $(SRC)

clean:
	rm -f minictl
