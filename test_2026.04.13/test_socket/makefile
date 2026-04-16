.PHONY:all

all: server client

server: Server.cpp
	g++ -g $^ -o $@

client: Client.cpp
	g++ -g $^ -o $@

.PHONY:clean
clean:
	rm -f server client
