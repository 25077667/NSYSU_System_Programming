
FLAGS=--std=gnu11 -Wall -Wextra --static

all: dep
	gcc *.o main.c ${FLAGS} -o target

dep: systemv.c
	gcc systemv.c ${FLAGS} -c

clean:
	rm target *.o