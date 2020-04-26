all: server client
	
server: 
	gcc Server/server.c -o Server/server
client: 	
	gcc Client/client.c -o Client/connect
clean:
	rm Server/server Client/connect
