#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "message.h"

#define BUF_SIZE 512

int main(int argc, char *argv[]) {
    struct sockaddr_in server;
    int len = sizeof(struct sockaddr_in);
    char buffer[BUF_SIZE];
    struct hostent *host;
    int recvln, sockfd, port, i, fd;
size_t bytes_recv, file_size;
int count = 0; 

struct msg_t message;
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <host> <port> <filename>\n", argv[0]);
	return 1;
    }

    host = gethostbyname(argv[1]);
    if (host == NULL) {
	perror("gethostbyname");
	return 1;
    }

    port = atoi(argv[2]);

    /* initialize socket */
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	perror("problem initializing socket");
	exit(0);
    }

    /* initialize server addr */
    memset((char *) &server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr*) host->h_addr);

fprintf(stderr, "sending packets begins\n");

    /* send message */
fprintf(stderr, "Enter the file you want to request from server\n");
	//scanf("%s", argv[3]);
	//sprintf(buffer,"%s\n", argv[3]);

    if (sendto(sockfd, argv[3], strlen(argv[3]), 0, (struct sockaddr *) &server, len) < 0) {
	perror("problem sending()");
	exit(0);
    }

	if((recvfrom(sockfd, (ssize_t*)&file_size,sizeof(file_size), 0, NULL, NULL)) < 0){
	fprintf(stderr, "Error reading the size of the file\n");
	}
	fprintf(stderr, "The size of the file is: %zd\n", file_size);
	
	if((bytes_recv = recvfrom(sockfd, message.payload, BUF_SZ, 0, NULL, NULL)) < 0){
	perror("Error receiving bytes from the other side\n");
	}
	if((fd = open(argv[3], O_WRONLY|O_CREAT, 0644)) < 0){
	fprintf(stderr, "Error creating file\n");	
	}
	
	if(write(fd, message.payload, 512) < 0){
	perror("Error writing to file\n");
	}



   
    close(sockfd);
    return 0;
}

