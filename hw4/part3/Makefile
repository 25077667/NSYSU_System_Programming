FLAGS= -Wall -std=gnu11

all: dict dict.h main.c lookup1.c convert.c
	gcc ${FLAGS} -Ofast convert.c dict.h -o convert
	./convert dict myfixrec
	@echo gen myfixrec success!

	gcc ${FLAGS} -g lookup1.c main.c -o file_lookup

clean:
	rm myfixrec convert file_lookup