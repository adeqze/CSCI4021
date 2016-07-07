# CSCI4021
This is the README for Homework 2 CSCI4211 Fall 2015.

**********************************
Author:
**********************************	
Mohamed Yunis : noorx004@umn.edu

**********************************
CSE Machine code tested on:
**********************************
KH4250-33

**********************************
Files:
**********************************
README : This file

server_tcp.c:	Server Program: The file that contains the TCP code for the server
server_udp.c:  Server program:  The file that contains the UDP code for the server

client_tcp.c:	Client program: The file containing the TCP code for the client
client_udp.c:	Client program: The file containing the UDP code for the client 

makefile: 	File for compiling the source codes. 

********************
Syntax and Usage:
********************
The server and client would be run from two different terminals. They can be run on same machine or different machines
on the same network. Below is how to run the programs:
Since a makefile is included for the prorgams, you can just type "make" or "make all" at terminal at the location of the source code 
This will compile all code.
Make sure to do "make" at both server(s) and clients(s) locations.  

**************
Server usage
**************

./server_tcp <port> or ./server_udp <port>

Below is the explanation of the usage:

Server: is the excutable containing the server code
port: is the port number to listen for connection on


***************
client usage
***************

./client <ip_address> <port> <filename> or ./client_udp <ip_address> <port> <filename>

Below is the explanation of the client usage:

client: is the name of the excutable client program
ipaddress: is the ipaddress of the machine the server is running on
port: is the port the server is listening on. It has to be the same port the put as an argument in the server program
filename: the name of the file you want to request to download from the server. 

 
To compile the server and client with the provided Makefile, type at the terminal:

>make clean //if you want to clean the .o files off the current location
>make all or make

Note: There is a specific make for client programs and one for server programs as pointed out in the handout. 

Note: when putting the IP address option. You can either put it the ip address of the server computer or just call it by name 
	since if testing on the school computeres. For instace, "kh4250-0x" or "lind40-0x"



