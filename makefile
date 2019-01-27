CFLAGS = -c -g -Wall

all: http_client http_server client

http_client: http_client.o
	gcc -o http_client http_client.o -lm

http_server: http_server.o
	gcc -o http_server http_server.o -lm
	
http_client.o: http_client.c
	gcc $(CFLAGS) http_client.c
	
http_server.o: http_server.c
	gcc $(CFLAGS) http_server.c

clean:
		rm -f *.o http_client http_server