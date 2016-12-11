default: server client

server: server.c
	gcc -c server.c -o server

client: client.c
	gcc -c client.c -o client
