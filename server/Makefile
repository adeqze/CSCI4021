programs = server_tcp server_udp

all: clean $(programs)

server_tcp:
	gcc server_tcp.c -o server_tcp
server_udp:
	gcc server_udp.c -o server_udp
	
clean:
	rm -f $(programs) *.o
	
