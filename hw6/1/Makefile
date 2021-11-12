FLAGS=--std=gnu11 -Wall -Wextra -Werror --static

all:
	gcc main.c lib.o ${FLAGS} -Ofast -lstdc++ -o target 
	strip target

dbg:
	gcc ${FLAGS} -D__DEBUG__ -g -o target main.c

clean:
	rm target