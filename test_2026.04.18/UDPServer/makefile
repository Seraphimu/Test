.PHONY:all
all: client server
client: Client.cpp
	g++ -g $^ -o $@
server: Server.cpp
	g++ -g $^ -o $@
.PHONY:clean

clean:
	rm -f server client