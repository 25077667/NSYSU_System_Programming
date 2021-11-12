CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=gnu11 -g
OUT = myshell

all: dep
	$(CC) $(CFLAGS) *.o -o $(OUT)

dep:
	$(CC) $(CFLAGS) -c shell.h builtin.c parser.c redirect.c run_command.c
	$(CC) $(CFLAGS) -c shell.[ch]

clean:
	rm -f $(OUT) *.o *.gch peda-session-*.txt .gdb_history

info:
	cloc .

help:
	@printf "make clean dep all:                              to make clean and all\n"
	@printf "make info:          to get the number of lines(Will add more features)\n"
