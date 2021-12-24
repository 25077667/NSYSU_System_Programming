FLAGS=--std=gnu11 -Wall -Wextra -Werror -Ofast
O_CLI=usock_cilent
O_SER=usock_server

all: ${O_CLI} ${O_SER}
	# strip ${O_CLI}  
	# strip ${O_SER}

${O_SER}: lookup5.o usock_server.c
	gcc lookup5.o usock_server.c -o ${O_SER} ${FLAGS}

${O_CLI}: lookup7.o main.c
	gcc lookup7.o main.c -o ${O_CLI} ${FLAGS}

lookup5.o: lookup5.c
	gcc lookup5.c ${FLAGS} -c

lookup7.o: lookup7.c
	gcc lookup7.c ${FLAGS} -c

clean:
	rm ${O_CLI} ${O_SER} *.o