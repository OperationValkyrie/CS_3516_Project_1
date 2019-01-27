/**
 * Jonathan Chang
 * CS 3516 A01 A18
 * Project 1
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char** argv) {
	struct addrinfo thisInfo;
	struct addrinfo* serverInfo;
	struct sockaddr_in client;

	char* port_number;
	char* message = (char*) malloc((50000)*sizeof(char));
	int socketTwo;

	if(argc != 2) {
		printf("%s\n", "Usage ./http_server port_number");
		return 1;
	}
	port_number = argv[1];

	memset(&thisInfo, 0, sizeof (thisInfo));
	thisInfo.ai_family = AF_INET;
	thisInfo.ai_socktype = SOCK_STREAM;

	// Get addressinfo
	if(getaddrinfo(NULL, port_number, &thisInfo, &serverInfo)) {
		printf("%s\n", "Unable get address info.");
		exit(1);
	}
	printf("%s\n", "Got address info.");

	// Open socket
	int socketOne = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if(socketOne < 0) {
		close(socketOne);
		printf("%s\n", "Unable to open socket.");
		exit(1);
	}
	printf("%s\n", "Opened Socket.");

	// Bind
	if(bind(socketOne, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
		close(socketOne);
		printf("%s\n", "Unable to bind.");
		return 1;
	}
	printf("%s\n", "Binded.");

	// Run server
	while(1) {
		if(listen(socketOne, 1) < 0) {
			printf("%s\n", "Listening error.");
		}

		char* temp = (char*) malloc((5000)*sizeof(char));

		socketTwo = accept(socketOne, (struct sockaddr *)&client, (socklen_t*)temp);
		if(socketTwo < 0) {
			printf("%s\n", "Accept error.");
		}

		int receive = recv(socketTwo, temp, 5000, 0);
		if(receive < 0) {
			printf("%s\n", "Receive error.");
			printf("%s\n", strerror(errno));
		}

		//printf("%s\n", temp);
		message = (char*) malloc((5000)*sizeof(char));

		char* token = (char*) strstr(temp, "/");
		char* filepath = (char*) malloc((5000)*sizeof(char));;
		strncpy(filepath, token, 10);
		filepath[10] = '\0';
		if(strcmp(filepath, "/TMDG.html") != 0 && strcmp(filepath, "/ HTTP/1.1") != 0) {
			strcat(message, "HTTP/1.1 404 NOT FOUND\r\n\r\n");
			if(send(socketOne, message, strlen(message), 0) < 0) {
				printf("%s\n", "Unable to send.");
			}
		} else {
			printf("%s\n", "Sending message");

			char buf[1024];
			size_t n;
			FILE *fil;
			fil= fopen("TMDG.html", "r");

			FILE *stream;
			if((stream = fdopen(socketTwo, "r+")) == NULL) {
				printf("%s\n", "Error on stream");
			}

			if(fil) {
				fprintf(stream, "HTTP/1.1 200 OK\r\n\r\n");
				while((n = fread(buf, 1, sizeof(buf), fil) > 0)) {
					// Send message
					fprintf(stream, buf);
				}
			}
			fflush(stream);
			fclose(stream);
		}
		close(socketTwo);
		printf("%s\n", "Served Request");
	}
	printf("%s\n", "Closing");
	close(socketOne);
	close(socketTwo);
	return 0;
}
