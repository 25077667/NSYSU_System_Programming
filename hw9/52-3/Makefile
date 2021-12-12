FLAGS=--std=gnu11 -Wall -Wextra -Werror --static -Ofast

all: server client
	strip server 
	strip client

server: dep pmsg_file_server.c
	gcc get_num.o error_functions.o pmsg_file_server.c -lrt ${FLAGS} -o server

client: dep pmsg_file_client.c
	gcc get_num.o error_functions.o pmsg_file_client.c -lrt ${FLAGS} -o client

dep: get_num.o error_functions.o  

get_num.o: get_num.c
	gcc get_num.c ${FLAGS} -c

error_functions.o: error_functions.c
	gcc error_functions.c ${FLAGS} -c

clean:
	rm server client *.o