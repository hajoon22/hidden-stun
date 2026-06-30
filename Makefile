example: server.o client.o stun.o hidden.o
	gcc -o client client.o stun.o hidden.o	
	gcc -o server server.o stun.o

server.o: src/example/server/main.c
	gcc -c src/example/server/main.c -o server.o
client.o: src/example/client/main.c
	gcc -c src/example/client/main.c -o client.o
hidden.o: src/hidden/hidden.c src/hidden/hidden.h
	gcc -c src/hidden/hidden.c -o hidden.o
stun.o: src/hidden/stun/stun.c src/hidden/stun/stun.h
	gcc -c src/hidden/stun/stun.c -o stun.o

clean:
	rm -rf *.o server client
