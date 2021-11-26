FLAGS=--std=gnu11 -Wall -Wextra -Werror --static -Ofast

all: server client
	strip server 
	strip normal_client 
	strip dos_client

server: dep
	gcc get_num.o error_functions.o fifo_seqnum_server.c ${FLAGS} -o server

client: dep
	gcc get_num.o error_functions.o fifo_seqnum_client.c ${FLAGS} -o normal_client
	gcc get_num.o error_functions.o dos_client.c ${FLAGS} -o dos_client

dep: get_num.o error_functions.o  

get_num.o: get_num.c
	gcc get_num.c ${FLAGS} -c

error_functions.o: error_functions.c
	gcc error_functions.c ${FLAGS} -c

clean:
	rm server normal_client dos_client *.o