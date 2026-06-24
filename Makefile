server: server.o stun.o
	gcc -o server server.o stun.o
server.o: src/server/main.c
	gcc -c src/server/main.c -o server.o

client: client.o stun.o
	gcc -o client client.o stun.o
client.o: src/client/main.c
	gcc -c src/client/main.c -o client.o
	
stun.o: src/stun/stun.c src/stun/stun.h
	gcc -c src/stun/stun.c -o stun.o

clean:
	rm -rf *.o server client
