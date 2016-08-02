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

#define BUF_SZ 256

void replace(char *filename, char *Filename);

int main(int argc, char *argv[])
{  
 	int sockfd, newsockfd, portno, n;
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	char buffer[BUF_SZ], buffer_send[BUF_SZ];
	int i = 0, fd; 
	char *listOfServers[5];
	char *filename = "listOfServers.txt";
	char *Filename = "list_of_servers.txt";
FILE *fp;
char *buf_ptr, *find_ptr;

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

	char buffer_ip[256];
	char buffer_port[256];
while(1){
	listen(sockfd, 5);
	fprintf(stderr, "Listening for clients.....\n");
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd < 0)
	{
		perror("error accepting connection\n");
	}

	fprintf(stderr, "Accepted a client connection from %s on port : %d\n", inet_ntoa(cli_addr.sin_addr), portno);
	fprintf(stderr, "Now getting ready to get messages from cliednt\n");
	
	
	//bzero(buffer, 256);
	n = read(newsockfd, buffer, 256);
	if(n < 0){perror("error reading from socket\n");}
	fprintf(stderr, "The message from the client: %s \n", buffer);

	if(strstr(buffer, "register") != NULL){
	fprintf(stderr, "Register message received\n");
	n = write(newsockfd, "Success\r\n", 10);
	
	if((fd = open(filename, O_CREAT|O_RDWR|O_APPEND, 0644)) < 0){
	fprintf(stderr, "Error creating file\n");
	}
	fprintf(stderr, "The message in buffer now: %s\n", buffer);
//lseek(fd, 0, SEEK_END);
	n = write(fd, buffer, 26);
	if(n < 0){perror("Writing to file:");}

/*..Now give me info required for client connection to you...*/
	fprintf(stderr, "\n");
	fprintf(stderr, "Now getting info required by client to connect to app_server\n");

	
	n = read(newsockfd, buffer_ip, 256);
	if(n < 0){perror("error reading ip for client later: ");}
	n = read(newsockfd, buffer_port, 256);
	if(n < 0){perror("error reading port for client use: ");}
	
	fprintf(stderr, "The  ip number for client use is: %s\n", buffer_ip);
	fprintf(stderr, "The port number for client use is: %s\n", buffer_port);


	}
//if asked for servers list
	if(strstr(buffer, "list-servers") != NULL){
	fprintf(stderr, "list-server message received\n");

	fprintf(stderr, "Providing IP and port needed to connect to app-server later\n");
	fprintf(stderr, "The IP to provide to client: %s\n", buffer_ip);
	fprintf(stderr, "The port to provide to client: %s\n", buffer_port);
	n = write(newsockfd, buffer_ip, 256);
	if(n < 0){perror("Error providing IP to client: ");}
	n = write(newsockfd, buffer_port, 256);
	if(n < 0){perror("Error providing port to client: ");}

	replace(filename, Filename);
	
	fp = fopen(Filename, "rb");
	if(fp == NULL){perror("Opening for replace: ");}
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);	
	
	char *string = malloc(fsize + 1);
	fread(string, fsize, 1, fp);
	fclose(fp);
	string[fsize] - 0;
	
	fprintf(stderr, "Content string: %s\n", string);

	n = write(newsockfd, string, BUF_SZ);
	if(n < 0){perror("Error writing to other side: ");}
	
	
	/*
	n = fread(buffer_send,sizeof(buffer_send), 1, fp1);
	if(n < 0){perror("Reading into buffer_send:");}
	fprintf(stderr, "Buffer_send contents: %s\n", buffer_send);
*/

/*
n = read(buffer_send, fd, 26);
if(n < 0){perror("Error reading file into buffer_send: ");}
fprintf(stderr, "The contents of buffer_send: %s\n", buffer_send);
sprintf(buffer_send, "%s %d ", "success", fd);
n = write(newsockfd, buffer_send, 256);if(n<0){perror("Error writing buffer_send:");}
*/ 
	
	}
	else{
	n = write(newsockfd, "Failure", 10);
	if(n < 0){perror("Error writing failure message\n");}
	}
	}
		
	close(fd);
	close(sockfd);
	close(newsockfd);
return 0;

}
#include <stdio.h>
#include <string.h>

#define MAX_LEN_SINGLE_LINE     120   // ?

void replace(char *filename, char *Filename){
    const char fileOrig[32] = "listOfServers.txt";
    const char fileRepl[32] = "list_of_servers.txt";
    const char text2find[80] = "register";
    const char text2repl[80] = "success";

    char buffer[MAX_LEN_SINGLE_LINE+2];
    char *buff_ptr, *find_ptr;
    FILE *fp1, *fp2;
    size_t find_len = strlen(text2find);

    fp1 = fopen(fileOrig,"r");
    fp2 = fopen(fileRepl,"w");

    while(fgets(buffer,MAX_LEN_SINGLE_LINE+2,fp1))
    {
        buff_ptr = buffer;
        while ((find_ptr = strstr(buff_ptr,text2find)))
        {
            while(buff_ptr < find_ptr)
                fputc((int)*buff_ptr++,fp2);

            fputs(text2repl,fp2);

            buff_ptr += find_len;
        }
        fputs(buff_ptr,fp2);
    }

    fclose(fp2);
    fclose(fp1);
}



