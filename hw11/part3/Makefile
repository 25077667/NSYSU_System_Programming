FLAGS=--std=gnu11 -Wall -Wextra -Werror -g
O_CLI=udp_client
O_SER=udp_server

all: ${O_CLI} ${O_SER}
	# strip ${O_CLI}  
	# strip ${O_SER}

${O_SER}: lookup5.o udp_server.c
	gcc lookup5.o udp_server.c -o ${O_SER} ${FLAGS}

${O_CLI}: lookup9.o main.c
	gcc lookup9.o main.c -o ${O_CLI} ${FLAGS}

lookup5.o: lookup5.c
	gcc lookup5.c ${FLAGS} -c

lookup9.o: lookup9.c
	gcc lookup9.c ${FLAGS} -c

clean:
	rm ${O_CLI} ${O_SER} *.o