FLAGS=--std=gnu11 -Wall -Wextra -Werror -Ofast
O_CLI=isock_cilent
O_SER=isock_server

all: ${O_CLI} ${O_SER}
	strip ${O_CLI}  
	strip ${O_SER}

${O_SER}: lookup5.o isock_server.c
	gcc lookup5.o isock_server.c -o ${O_SER} ${FLAGS}

${O_CLI}: lookup8.o main.c
	gcc lookup8.o main.c -o ${O_CLI} ${FLAGS}

lookup5.o: lookup5.c
	gcc lookup5.c ${FLAGS} -c

lookup8.o: lookup8.c
	gcc lookup8.c ${FLAGS} -c

clean:
	rm ${O_CLI} ${O_SER} *.o