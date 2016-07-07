#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>


#include "message.h"
int file_sending(int sock, char *filename);

int main(int argc, char *argv[])
{  
 	int sockfd, newsockfd, portno, n;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	char buffer[BUF_SZ];
	struct msg_t message_buffer; 


struct timeval start, end;

if(argc != 2){
 fprintf(stderr,"usage %s port\n", argv[0]);
exit(0);
}
	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Socket");
		exit(0);
	}
	else{
	fprintf(stderr, "Server created\n");
	}
	/*---Initialize address/port structure---*/
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = portno;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0 )
	{
		perror("Connecting to Socket");
		exit(0);
	}
	else{
	fprintf(stderr, "Server socket bound to port: %d\n", portno);
	}

	/*---Make it a "listening socket"---*/
	listen(sockfd, 1);
	fprintf(stderr, "Listening for clients.....\n");
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
	{
		perror("error accepting\n");
	}
	else{
	fprintf(stderr, "Accepted a client connection from %s on port : %d\n", inet_ntoa(cli_addr.sin_addr), portno);
	}
	
	bzero(message_buffer.payload, BUF_SZ);	
	n = recv(newsockfd, message_buffer.payload, BUF_SZ, 0);
	fprintf(stderr, "Name of file requested: %s\n", message_buffer.payload);
	send(newsockfd,  "I received your message", 255,0);
	
	file_sending(newsockfd, message_buffer.payload);	
	//struct msg_t something; 
	//something.msg_type = MSG_TYPE_GET;
	//fprintf(stderr, "First enum: %s\n", str_map[MSG_TYPE_GET]);
	//char *filename = message_buffer.payload;
	//FILE *fd = fopen(filename,  "r");
	//if(fd < 0){
	//perror("Error opening file:");
	//exit(0);
	//}

//struct stat stat_buf;
//fstat(fd, &stat_buf); //the size of the file 

	//else{
		//fprintf(stderr, "sending file contents to you:\n");
		//while(fgets(message_buffer.payload, BUF_SZ, fd) != NULL){
		//send(newsockfd, message_buffer.payload, BUF_SZ, 0);
//}
//}  


close(sockfd);
close(newsockfd);
return 0;

}

int file_sending(int sock, char *filename){

struct msg_t message;
ssize_t bytes_read, bytes_sent, number_of_bytes;

int fd;

message.cur_seq = 0;
number_of_bytes = 0;
//file opening

if((fd = open(filename, O_RDONLY)) < 0){
fprintf(stderr, "%s\n", str_map[MSG_TYPE_GET_ERR]);
}

if((bytes_sent = send(sock, str_map[MSG_TYPE_GET_ERR], strlen(str_map[MSG_TYPE_GET_ERR]), 0)) < 0){
perror("File sending error: ");
}
else{
fprintf(stderr, "%s\n", str_map[ MSG_TYPE_GET_RESP]);
while((bytes_read = read(fd, message.payload, BUF_SZ)) > 0){
if((bytes_sent = send(sock, message.payload, bytes_read, 0)) < bytes_read){
perror("File sending error: ");
}
message.cur_seq++;
number_of_bytes += bytes_sent;
}
close(fd);
}

fprintf(stderr, "Sent: %zd bytes: \n", number_of_bytes);
fprintf(stderr, "%s\n", str_map[ MSG_TYPE_GET_ACK]);
return message.cur_seq;
}



