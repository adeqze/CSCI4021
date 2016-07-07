programs = client_tcp client_udp

all: clean $(programs)

client_tcp: 
	  gcc client_tcp.c -o client_tcp
client_udp:
	  gcc client_udp.c -o client_udp
clean:
	rm -f $(programs) *.o
