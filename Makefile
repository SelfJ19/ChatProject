all: server client
	
ref: server_ref client_ref

server_ref: server_testing/stuserver.c
	gcc -o server server_testing/stuserver.c

client_ref: server_testing/stuclient.c
	gcc -o client server_testing/stuclient.c

server: server.c	
	gcc -o server server.c

client: client.c
	gcc -o client client.c

clean:
	rm server
	rm client
