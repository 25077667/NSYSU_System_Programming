TARGET=myshell
FLAGS=-std=gnu11 -Wall

all: dep
	gcc ${FLAGS} *.o shell.c -Ofast -o ${TARGET}
	strip ${TARGET}

dep: *.c *.h
	gcc ${FLAGS} shell.h builtin.c parse.c run_command.c is_background.c -c

check: all
	valgrind --verbose --leak-check=full --show-leak-kinds=all ./${TARGET}

clean:
	rm -f *.o *.gch ${TARGET}