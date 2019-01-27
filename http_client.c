/**
 * Jonathan Chang
 * CS 3516 A01 A18
 * Project 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
/**
 * Prints out the usage information
*/
void printHelp() {
    printf("%s\n", "Usage ./http_client [-p] server_url port_number");
    printf("%s\n", "Options");
    printf("%s\n", "  -p           Optional verbose flag");
    printf("%s\n", "  server_url   The server url or IP address");
    printf("%s\n", "  port_number  The port number on which to contact the server");
}

/**
 * Makes a http request to the given url and port number
*/
int main(int argc, char** argv) {
	unsigned int verbose_flag = 0;
	char* server_url = (char*) malloc((500)*sizeof(char));
	char* port_number = (char*) malloc((500)*sizeof(char));

	struct addrinfo thisInfo;
	struct addrinfo* serverInfo;
	char* message = (char*) malloc((500)*sizeof(char));
	char* filepath = (char*) malloc((5000)*sizeof(char));
	char* urlSplit;

	char c;
	unsigned int argn = 1;

	// Read in first optional argument
	while((c = getopt(argc, argv, "p")) != -1) {
	switch(c) {
		case 'p':
			verbose_flag = 1;
			argn++;
			break;
		default:
			printHelp();
			exit(0);
		}
	}
	//Ensure proper number of required arguments
	if(argc - argn != 2) {
		printHelp();
		exit(0);
	}

	// Read in other arguments
	char* urlArg = argv[argn++];
	strncpy(server_url, urlArg, strlen(urlArg));
	char* portArg = argv[argn++];
	strncpy(port_number, portArg, strlen(portArg));

	urlSplit = strtok(server_url, "/");

	int urlLen = strlen(argv[argn - 2]);
	int hostLen = strlen(urlSplit);
	char* url = argv[argn - 2];
	strncpy(filepath, &url[hostLen], urlLen = hostLen);

	printf("%s%d\n", "verbose_flag: ", verbose_flag);
	printf("%s%s\n", "server_url:   ", server_url);
	printf("%s%s\n\n", "port_number:  ", port_number);

	memset(&thisInfo, 0, sizeof (thisInfo));
	thisInfo.ai_family = AF_INET;
	thisInfo.ai_socktype = SOCK_STREAM;

	// Get addressinfo
	if(getaddrinfo(urlSplit, port_number, &thisInfo, &serverInfo)) {
		printf("%s\n", "Unable get address info.");
		exit(1);
	}
	printf("%s\n", "Got address info.");

	// Open socket
	int socketOne = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);
	if(socketOne < 0) {
		printf("%s\n", "Unable to open socket.");
		exit(1);
	}
	printf("%s\n", "Opened Socket.");

	// Set start time if RTT
	long long unsigned int startTime = clock();

	printf("%s\n", "Attempting to connect.");
	// Attempt to connect to address
	if(connect(socketOne, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0) {
		printf("%s\n", "Unable to connect.");
		exit(1);
	}
	printf("%s\n", "Connected.");

	// Set end time if RTT
	long long unsigned int endTime = clock();

	if(strlen(filepath) == 0) {
		filepath = "/";
	}
	strcat(message, "GET ");
	strcat(message, filepath);
	strcat(message, " HTTP/1.1\r\n");
	strcat(message, "Host: ");
	strcat(message, urlSplit);
	strcat(message, "\r\n");
	strcat(message, "Connection: keep-alive\r\n\r\n");
	printf("\n%s\n", message);

	// Send message
	if(send(socketOne, message, strlen(message), 0) < 0) {
		printf("%s\n", "Unable to send.");
		exit(1);
	}
	printf("%s\n", "Message sent.");
	int i = 1;
	int count = 0;
	char* singleReply = (char*) malloc((50000)*sizeof(char));
	while(i > 0) {
		i = recv(socketOne, singleReply, 50000, 0);
		char* point = (char*) strstr(singleReply, "\r\n\r\n");
		char* one = (char*) strstr(singleReply, "</html>");
		if(one != NULL) {
			i = 0;
			printf("%s\n\n", "WHAT");
			printf("%s\n", singleReply);
			printf("%s\n", one);
			printf("%.*s\n", (int) (strlen(singleReply) - strlen(one) + 7), singleReply);
		} else if(point != NULL) {
			printf("%s", point);
		} else {
			printf("%s", singleReply);
		}
		if(count == 0 && strstr(singleReply, "200 OK") == NULL) {
			i = 0;
		}
		count++;
	}

	printf("\n%s\n", "Message received.");

	if(verbose_flag) {
		int time = endTime - startTime;
		printf("%d %s\n", time, "miliseconds");
	}

	return 0;
}
