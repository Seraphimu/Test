.PHONY:all

all: server client

server: Server.c
	gcc -g $^ -o server
client: Client.c
	gcc -g $^ -o client

.PNONY:clean

clean:
	rm -f server client
