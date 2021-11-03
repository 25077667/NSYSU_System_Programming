FLAGS=--std=gnu11 -Wall -Wextra -Werror --static

all:
	gcc ${FLAGS} -Ofast -o target main.c
	strip target

dbg:
	gcc ${FLAGS} -g -o target main.c

clean:
	rm target