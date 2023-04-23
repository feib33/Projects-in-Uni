#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr;
	char buffer[2048], *request[3];

	if (argc < 2) 
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	/* Ceate TCP socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		perror("ERROR opening socket");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	portno = atoi(argv[1]);

	/* Create address we're going to listen on (given port number)
	 - converted to network byte order & any IP address for 
	 this machine */
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);  // store in machine-neutral format

	 /* Bind address to the socket */
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
	{
		perror("ERROR on binding");
		exit(1);
	}

	/* Listen on socket - means we're ready to accept connections - 
	 incoming connection requests will be queued */

	listen(sockfd, 5);

	newsockfd = accept(sockfd, NULL, NULL);
	
	if (newsockfd < 0) 
	{
		perror("ERROR on accept");
		exit(1);
	}

	if (recv(newsockfd, buffer, 2048, 0) < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}


	request[0] = strtok(buffer, "\n");
	if(strcmp(strtok(request[0], " "), "GET") = 0) {
		request[1] = strtok(NULL, " ");
		request[2] = strtok(NULL, "\n")
	} else {
		send(newsockfd, "only GET supported", 18, 0);
	}




	close(sockfd); // close socket
	return 0;
}