CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = `pkg-config --libs dbus-1`
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

client: client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)
