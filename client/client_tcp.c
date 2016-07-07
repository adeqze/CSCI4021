#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>


#include "message.h"

int file_receiving(int sock, char *filename);
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

struct msg_t thisBuffer;



    if (argc != 4) {
       fprintf(stderr,"usage %s hostname port filename\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = portno;
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        error("ERROR connecting");
}

	//fprintf(stderr, "Enter name of file you want to download\n");	
	//bzero(thisBuffer.payload,BUF_SZ);
	fprintf(stderr, "Argument 3 is: %s\n?", argv[3]);
	//scanf(argv[3], thisBuffer.payload);
	//strcpy(argv[3], thisBuffer.payload);
	
	
	send(sockfd, argv[3], strlen(argv[3]), 0);
	recv(sockfd, thisBuffer.payload,BUF_SZ, 0);
	fprintf(stderr, "Acknowledgment: %s\n", thisBuffer.payload);

//start actual file sending from this point after request.
 //for gettime function 
clock_t start, end;
double time_spent;
start = clock();
file_receiving(sockfd, argv[3]);
end = clock();

time_spent =(double) (end - start);
printf("Time difference for receiving is: %f\n", time_spent/60);

    close(sockfd);
    return 0;
}

int file_receiving(int socket, char *filename){

struct msg_t message;
ssize_t bytes_sent, bytes_recv, recv_size;
char buffer_send[BUF_SZ];
//char buffer_recv[BUF_SZ];
size_t length_transmitted;
int fd;

//sprintf(buffer_send, "%s\n", filename);
//length_transmitted = strlen(buffer_send);

//if((bytes_sent = send(socket, filename, length_transmitted, 0)) < 0){
//perror("File sending error: ");
//exit(0);
//}
//file to be saved on current working directory
if((fd = open(filename, O_WRONLY|O_CREAT, 0644)) < 0){
fprintf(stderr, "Error creating file\n");
}
message.cur_seq = 0;
recv_size = 0;
bzero(message.payload, BUF_SZ);
while((bytes_recv = recv(socket, message.payload,BUF_SZ, 0)) > 0){

message.cur_seq++;
recv_size += bytes_recv;

if(write(fd, message.payload, bytes_recv) < 0){
perror("writing to file error\n");
}
}
close(fd);
fprintf(stderr, "< %s >  < %d > < %d > < %zd > \n",  str_map[MSG_TYPE_GET_ACK],  message.cur_seq, message.max_seq , strlen(message.payload));
fprintf(stderr, "Received: %zd bytes: \n", recv_size);
return recv_size;
}
