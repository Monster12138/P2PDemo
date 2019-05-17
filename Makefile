all:server clients
server:server.c
	gcc $^ -o $@
clients:client1 client2
client1:client1.c
	gcc $^ -o $@
client2:client2.c
	gcc $^ -o $@
clean:
	rm -rf server client1 client2
