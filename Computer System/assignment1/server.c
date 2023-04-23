/*  
		A simple HTTP server by using port number and path as argument,
	send HTTP response message according to Get requests and make use 
	of Pthread to handle multiple requests. A part of codes in main
	function were quoted from LMS Sample code (Lab 5 Source Files server.c).

	Author:     Fei Bao
	LoginID:    feib
	StudentID:  804522

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_NUM_BYTES 2048


char g_path[MAX_NUM_BYTES]; // global variable of path

/* declare functions */
void* response_client(void *sock_desc);
void serve_content(char *request[3], char *send_buffer);

/* Read HTTP requests and send HTTP response message
	and handling multiple requests by Pthreads*/
int main(int argc, char const *argv[])
{
	int sockfd, newsockfd, portno;
	struct sockaddr_in serv_addr;
	pthread_t thread_id;


	if (argc < 2) 
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	if (argc == 3)
	{
		strcpy(g_path, argv[2]);
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

	while((newsockfd = accept(sockfd, NULL, NULL)) >= 0) {
		if(pthread_create(&thread_id, NULL, response_client, &newsockfd) < 0) {
			perror("ERROR on creating thread");
			exit(1);
		}	
		pthread_join(thread_id, NULL);
	}

	close(sockfd); // close socket
	return 0;
}



/* receive data from the connection and response headers according to different GET requests */
void* response_client(void* sock_desc) {
	char recv_buffer[MAX_NUM_BYTES], send_buffer[MAX_NUM_BYTES], *request[3];
	char body[MAX_NUM_BYTES], path[MAX_NUM_BYTES];
	int *newsockfd = (int *)sock_desc;
	int bytes_read;
	FILE *fp;

	
	if (recv(*newsockfd, recv_buffer, MAX_NUM_BYTES, 0) < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}
	
	strcpy(path, g_path); // assign global path to current path

	/* Divide GET line into three parts */
	request[0] = strtok(recv_buffer, " ");
	if(strncmp(request[0], "GET", strlen("GET")) == 0) {
		request[1] = strtok(NULL, " ");
		request[2] = strtok(NULL, "\n");

		/* server only support HTTP request */
		if(strncmp(request[2], "HTTP/1.0", strlen("HTTP/1.0")) != 0 
			&& strncmp(request[2], "HTTP/1.1", strlen("HTTP/1.1")) != 0) {
			send(*newsockfd, "HTTP/1.0 400 Bad Request\n\n", strlen("HTTP/1.0 400 Bad Request\n\n"), 0);
		} else {
			strcat(path, request[1]);
			fp = fopen(path, "r");
			if(fp == NULL) {
				send(*newsockfd, "HTTP/1.1 404 Not Found\n\n", strlen("HTTP/1.1 404 Not Found\n\n"), 0);
			} else {

				strcpy(send_buffer, "HTTP/1.0 200 OK\r\n");
				serve_content(request, send_buffer);
				send(*newsockfd, send_buffer, strlen(send_buffer), 0);

				/* keep reading more data */
				while((bytes_read = fread(body, sizeof(*body), MAX_NUM_BYTES, fp)) > 0) {
					send(*newsockfd, body, bytes_read, 0);
				}
			}
		}

	} else {
		send(*newsockfd, "only GET supported", strlen("only GET supported"), 0);
	}

	close(*newsockfd);
	return 0;
}

/* Respond to GET requests accourding to different content */
void serve_content(char *request[3], char *send_buffer) {
	if(strstr(request[1], ".html") != NULL) {
		strcat(send_buffer, "Content-Type: text/html\r\n\r\n");
	} else if (strstr(request[1], ".jpg") != NULL) {
		strcat(send_buffer, "Content-Type: image/jpeg\r\n\r\n");
	} else if (strstr(request[1], ".css") != NULL) {
		strcat(send_buffer, "Content-Type: text/css\r\n\r\n");
	} else if (strstr(request[1], ".js") != NULL) {
		strcat(send_buffer, "Content-Type: text/javascript\r\n\r\n");
	}
}

