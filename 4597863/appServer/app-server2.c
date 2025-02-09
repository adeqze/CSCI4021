#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SZ 256

int file_receiving(int socket, char *filename);
int main(int argc, char *argv[])
{  
 	int sockfd, sockfd1, newsockfd, portno, n, osport;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	char buffer[BUF_SZ];
	char buffer_send[BUF_SZ];
	int serv_len = sizeof(serv_addr);
int i;
struct hostent *host, *server;


char buffer_ip[256];
char buffer_port[256];

if(argc != 2){
 fprintf(stderr,"usage %s \n", argv[0]);
exit(0);
}


	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("erro opening Socket");
		exit(0);
	}
	
	/*---Initialize address/port structure---*/
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	//serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = portno;
	/*---Assign a port number to the socket---*/


	/*---Connect then register to the dir-server----*/

	/*added later to establish TCP with Apollo: until connect*/
	server = gethostbyname("apollo.cselabs.umn.edu");
	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

	if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        perror("ERROR connecting");
	}
	//Local machine information finding
	char hostname[128];
	gethostname(hostname, sizeof(hostname));
	fprintf(stderr, "My hostname is: %s\n", hostname);

	host = gethostbyname(hostname);
	fprintf(stderr, "My official name is: %s\n", host->h_name);
	fprintf(stderr, "My IP address is: %s\n", inet_ntoa(*(struct in_addr*)host->h_addr));
//end local machine info

	fprintf(stderr, "Connecting and Registering with dir-server\n");
	
	bzero((char *)&serv_addr, sizeof(serv_addr));
	//osport = rand() % (10000 + 1 - 9000) + 9000;
	osport = 9000 + rand() / (RAND_MAX /(10000 - 9000+1) + 1);
	serv_addr.sin_port = osport;
	
fprintf(stderr, "Did my IP address change and osport: %s %d\n", inet_ntoa(*(struct in_addr*)host->h_addr), osport);


	//strcpy(buffer, inet_ntoa(*(struct in_addr*)host->h_addr));
	sprintf(buffer, "%s %s %d %s %s ", "register", inet_ntoa(*(struct in_addr*)host->h_addr), osport, "\r", "\n");
	n = write(sockfd, buffer, 256);
	if(n < 0){perror("Error writing to socket of dir-server: ");}
	n = read(sockfd, buffer, 256);
	if(n < 0){perror("Error reading from socket: ");}

	fprintf(stderr, "The registration success message\n");
	fprintf(stderr, "%s\n", buffer);

	fprintf(stderr, "\n");


fprintf(stderr, "Now giving info required by app-client to connect to me\n");

/* just for connecting to app-client */


sprintf(buffer_ip, "%s",  inet_ntoa(*(struct in_addr*)host->h_addr));
sprintf(buffer_port, "%d", osport);

fprintf(stderr, "This will be used to connect by client\n");
fprintf(stderr, "IP address: %s\n", buffer_ip);
fprintf(stderr, "Port number: %s\n", buffer_port);

n = write(sockfd, buffer_ip, 256);
if(n < 0){perror("Error sending IP for client use: ");}
n = write(sockfd, buffer_port, 256);
if(n < 0){perror("Error sending port for client use: ");}


/*end connecting to app client later */


/*now Play the role of a server after registering */

fprintf(stderr, "Now I am going to play the role of a server\n");

	if ( (sockfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("erro opening Socket");
		exit(0);
		}
	fprintf(stderr, "Server created:\n");

    if (bind(sockfd1, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0 )
	{
		perror("error Connecting to Socket");
		exit(0);
	}
	
	
	fprintf(stderr, "I am now bound to OS assigned port: %d \n", osport);
		


	/*---Make it a "listening socket"---*/
	listen(sockfd1, 1);
	fprintf(stderr, "Listening for clients.....\n");
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd1, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
	{
		perror("error accepting\n");
	}
	else{
	fprintf(stderr, "Accepted a client connection from %s on port : %d\n", inet_ntoa(cli_addr.sin_addr), osport);
	}
	
	n = read(newsockfd, buffer, 255);
	fprintf(stderr, "The message from client is: %s \n", buffer); 
	n = write(newsockfd, "I got your message", 255);
	
	fprintf(stderr, "Now getting ready for file-receiving:\n ");
	
	/*start file receiving*/
	
	double total,avg_total;
	
	
	n = read(newsockfd, buffer, 16);
	if(n < 0){perror("Error reading 10kb: ");}
	
	fprintf(stderr, "File being received: %s\n", buffer);
	
	file_receiving(newsockfd, "10kb.txt");
	
	n = write(newsockfd, "I received 10kb.txt", 255);
	if(n < 0){perror("Erro confirming 10kb: ");}
	
	fprintf(stderr, "Fully received 10kb.txt\n");
	/*
	n = read(newsockfd, buffer, 255);
	if(n < 0){perror("Error reading 100kb: ");}
	fprintf(stderr, "buffer expecting 100: %s\n", buffer);
	if(strstr(buffer, "100kb.txt") != NULL){
	fprintf(stderr, "File being received %s\n", buffer);
	file_receiving(newsockfd, "100kb.txt");
	}
	
	n = read(newsockfd, buffer, 255);
	if(n < 0){perror("Error reading 1000kb: ");}
	fprintf(stderr, "buffer expecting 1000: %s\n", buffer);
	if(strstr(buffer, "1000kb.txt") != NULL){
	fprintf(stderr, "The file being received: %s\n", buffer);
	file_receiving(newsockfd, "1000kb.txt");
	}
	
	n = read(newsockfd, buffer, 255);
	if(n < 0){perror("Error reading 10kb: ");}
	fprintf(stderr, "buffer expecting 10000: %s\n", buffer);
	if(strstr(buffer, "10000kb.txt") != NULL){
	fprintf(stderr, "The file being Received %s\n", buffer);
	file_receiving(newsockfd, "10000kb.txt");
	}
*/


fprintf(stderr, "Do you get till here: \n");

close(newsockfd);
close(sockfd1);
close(sockfd);
return 0;

}

int file_receiving(int socket, char *filename){

char send_buffer[BUF_SZ], recv_buffer[BUF_SZ];
int f, recv_count; 
ssize_t bytes_sent, bytes_recvd, recv_fileSize;
size_t send_strlen;

sprintf(send_buffer, "%s\n", filename);
send_strlen = strlen(send_buffer);

if((bytes_sent = send(socket, filename, send_strlen, 0)) < 0){
perror("Sending error: ");
}

if((f = open(filename, O_WRONLY|O_CREAT, 0644)) < 0){
perror("Error creating file: ");
}
recv_count = 0;
recv_fileSize = 0;

while((bytes_recvd = recv(socket, recv_buffer, BUF_SZ, 0)) > 0){
recv_count++;
recv_fileSize += bytes_recvd;

if(write(f, recv_buffer, bytes_recvd) < 0){
perror("Error writing to file: ");
}
}
close(f);
fprintf(stderr, "Received %zu bytes: \n", recv_fileSize);
return recv_fileSize;

}  



