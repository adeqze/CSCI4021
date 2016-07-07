#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#define BUF_SIZE 512

#include "message.h"

int main(int argc, char* argv[]) {
    char buffer[BUF_SIZE];
	char buffer2[BUF_SIZE];
    struct sockaddr_in my_server, cli;
    int len = sizeof(struct sockaddr_in);
    int recvln, file, sockfd, port, fd, i;
	
struct msg_t message;
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	return 1;
    }

    /* initialize socket */
    if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	perror("socket");
	return 1;
    }

    /* bind to server port */
    port = atoi(argv[1]);
    memset((char *) &my_server, 0, sizeof(struct sockaddr_in));
    my_server.sin_family = AF_INET;
    my_server.sin_port = htons(port);
    my_server.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(sockfd, (struct sockaddr *) &my_server, sizeof(my_server)) < 0) {
	perror("Binding problem");
	exit(0);
    }
fprintf(stderr, "binding went okay:\n");
fprintf(stderr, "Waiting on connection and datagrams from client:\n");

bzero(buffer, BUF_SIZE);
if((recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &cli, &len)) < 0){
fprintf(stderr, "there was error receiving message\n");
}
	
	fprintf(stderr, "Connected to client: %s at port: %d\n", inet_ntoa(cli.sin_addr), port);
	fprintf(stderr, "File requested by client: %s is: %s\n",inet_ntoa(cli.sin_addr), buffer);

int fp;
size_t bytes_to_send, file_size;
if((fp = open(buffer, O_RDONLY)) < 0){
perror("Error opening file\n");
}
fprintf(stderr, "File access was okay. File sending will begin\n");
file_size = sizeof(fp);
fprintf(stderr, "The size of the file is: %zd\n", file_size);

if((sendto(sockfd, &file_size, sizeof(file_size), 0, (struct sockaddr *)&cli, len)) < 0){
fprintf(stderr, "Error sending the file size\n");
}

read(fp, buffer, sizeof(buffer));
if((bytes_to_send = sendto(sockfd, message.payload, BUF_SZ, 0, (struct sockaddr*)&cli, len)) < 0){
perror("Error sending\n");
}

fprintf(stderr, "If you didnt see the error message then file must have been sent\n");






    close(sockfd);
    return 0;
}

