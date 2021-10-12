TARGET=myshell
FLAGS=-std=gnu11 -Wall

all: dep
	gcc ${FLAGS} *.o shell.c -Ofast -o ${TARGET}
	strip ${TARGET}

dep: *.c *.h
	gcc ${FLAGS} -g shell.h builtin.c parse.c run_command.c is_background.c -c

dbg: dep
	gcc ${FLAGS} -g *.o shell.c -o ${TARGET}

check: all
	valgrind --verbose --leak-check=full --show-leak-kinds=all ./${TARGET}

clean:
	rm -f *.o *.gch ${TARGET}