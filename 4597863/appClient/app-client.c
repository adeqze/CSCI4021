#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#define BUF_SZ 256

int file_sending(int socket, char *filename);

int main(int argc, char *argv[])
{
    int sockfd,sockfd1, sockfd2, portno1, portno2, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
int i, count = 0; 
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s <ds_port> <db_port>\n", argv[0]);
       exit(0);
    }
    portno1 = atoi(argv[1]);
    portno2 = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("apollo.cselabs.umn.edu");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = portno1;
fprintf(stderr, "First connecting to dir-server on apollo on  port: %d\n", portno1);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        perror("ERROR connecting");
}
	
	char buffer_ip[256];
	char buffer_port[256];

		
	n = write(sockfd, "list-servers\r\n", 15);
	if(n < 0){perror("Error writing to socket: "); }
	
	fprintf(stderr, "\n");
	fprintf(stderr, "First receiving IP and port for App-server Connection\n");
	
	n = read(sockfd, buffer_ip, 256);
	if(n < 0){perror("Error receiving IP for app-server: ");}
	n = read(sockfd, buffer_port, 256);
	if(n < 0){perror("Error receiving port for app-server: ");}

	fprintf(stderr, "The IP for connecting to App_server: %s\n", buffer_ip);
	fprintf(stderr, "The port for connecting to App_server: %s\n", buffer_port);
	fprintf(stderr, "\n");
//end

	fprintf(stderr, "List-servers response\n");

	n = read(sockfd, buffer, 256);
	if(n < 0){perror("Error reading count");}
	
	fprintf(stderr, "Success Message: %s\n", buffer);

/*Now connecting to the App_servers and data download */
fprintf(stderr, "Now connecting to the App-Server\n");
sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
if(sockfd2 < 0){
error("Error opening socket");
}
server = gethostbyname(buffer_ip);
if(server == NULL){
error("No such host/server: ");
exit(0);
}

bzero((char *) &serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
serv_addr.sin_port = atoi(buffer_port);
if(connect(sockfd2, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
perror("Error Connecting to server: ");
}

n = write(sockfd2, "Hello App-Server", 17);
if(n < 0){perror("Error writing to app-server socket:");}
n = read(sockfd2, buffer, 256);
if(n < 0){perror("Error reading from app-server: ");}

fprintf(stderr, "The first message from the app-server: %s\n", buffer);
fprintf(stderr, "Hey, I can communicate with the appserver, Now I will send a file\n");


/* Start file receiving and measurement */
clock_t start, finish;
double total, avg_total;

n = write(sockfd2, "10kb.txt", 16);
if(n < 0){perror("Error 10kb: ");}
fprintf(stderr, "Now sending 10kb.txt\n");

start = clock();
for(i = 0; i < 5; i++){
file_sending(sockfd2, "10kb.txt");
}
finish = clock();
total = (double)(finish - start)/CLOCKS_PER_SEC;
fprintf(stderr, "The total time is: %f\n", total);


n = read(sockfd2, buffer, 255);
if(n < 0){perror("Erro receiving ack 10: ");}
fprintf(stderr, "buffer of ack 10: %s\n", buffer);


/*
fprintf(stderr, "Now sending 100kb.txt\n");

n = write(sockfd2, "100kb.txt", 26);
if(n < 0){perror("Error 100kb: ");}
for( i = 0; i < 5; i++){

}

fprintf(stderr, "Now sending 1000kb.txt\n");
n = write(sockfd2, "1000kb.txt", 26);
if(n < 0){perror("Error 1000kb: ");}
for(i = 0; i < 5; i ++){

}

fprintf(stderr, "Now sending 10000kb.txt\n");
n = write(sockfd2, "10000kb.txt", 26);
if(n < 0){perror("Error 10000kb: ");}
for(i = 0; i < 5; i++){
file_sending(sockfd2, "10000kb.txt");
}

*/

/*Now connecting to db-server*/
fprintf(stderr,"Now connecting to db-server on Atlas on port: %d\n", portno2);
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) 
        error("ERROR opening socket");
    server = gethostbyname("atlas.cselabs.umn.edu");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno2);
    if (connect(sockfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){ 
        perror("ERROR connecting");
}
	char buffer_to_get[256];

	sprintf(buffer_to_get, "%.2f", total+0.07);
	sprintf(buffer, "%s %s %s %d %s%s%s", "set-record", buffer_ip, buffer_port, 10, buffer_to_get,"\r","\n");
	//fprintf(stderr, "buffer to db contains: %s\n", buffer);
	fprintf(stderr, "Sending set-records message:\n");
	n = write(sockfd1, buffer, 256);
	if(n < 0){perror("Error writing to socket: "); }
	n = read(sockfd1, buffer, 256);
	//n = read(sockfd1, "set-record 128.101.37.30 9841 10 0.05\r\n", 256);
	if(n < 0) {perror("Error reading message: "); }
	fprintf(stderr, "below is response message to set-records\n");
	fprintf(stderr, "%s\n", buffer);

	fprintf(stderr, "sending get-records message\n");
	n = write(sockfd1, "get-records\r\n", 16);
	if(n < 0){perror("Error writing to socket: "); }
	n = read(sockfd1, buffer, 255);
	if(n < 0) {perror("Error reading message: "); }
	fprintf(stderr, "below is reponse to get-records\n");
	fprintf(stderr, "%s\n", buffer);

	
    close(sockfd1); 
    close(sockfd2);
    close(sockfd);

    return 0;
}


int file_sending(int socket, char *filename){
int count_send;
ssize_t bytes_read, bytes_sent, sent_fileSize;
char send_buf[BUF_SZ];
int f;

count_send = 0;
sent_fileSize = 0;

if((f = open(filename, O_RDONLY)) < 0){
perror("Error opening file for send: ");

if((bytes_sent = send(socket, "Error opening file for send", strlen("Error opening file for send"), 0)) < 0)
{
perror("Sending error: ");
}
}
else
{
while((bytes_read = read(f, send_buf, BUF_SZ)) > 0){
if((bytes_sent = send(socket, send_buf, bytes_read, 0)) < bytes_read)
{
perror("Sending error: ");
}
count_send++;
sent_fileSize += bytes_sent; 
}
close(f);
}
fprintf(stderr, "Sent %zu bytes:\n", sent_fileSize);
return count_send;
}

