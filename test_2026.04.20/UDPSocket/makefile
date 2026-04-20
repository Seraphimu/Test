.PHONY:all

all: server client

client: UDPClient.cpp
	g++ -g $^ -o $@
server: UDPServer.cpp
	g++ -g $^ -o $@
.PHONY:clean
clean:
	rm -f server client
