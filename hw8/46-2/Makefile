FLAGS=--std=gnu11 -Wall -Wextra -Werror --static -g

all: server client
	# strip server 
	# strip client

server: dep svmsg_file_server.c
	gcc get_num.o error_functions.o svmsg_file_server.c ${FLAGS} -o server

client: dep svmsg_file_client.c
	gcc get_num.o error_functions.o svmsg_file_client.c ${FLAGS} -o client

dep: get_num.o error_functions.o  

get_num.o: get_num.c
	gcc get_num.c ${FLAGS} -c

error_functions.o: error_functions.c
	gcc error_functions.c ${FLAGS} -c

clean:
	rm server client *.o