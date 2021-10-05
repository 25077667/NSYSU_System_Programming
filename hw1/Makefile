FLAGS=-std=c11 -Wall

all: 
	gcc ${FLAGS} -Ofast -o hw1-1 hw1-1.c
	gcc ${FLAGS} -Ofast -o hw1-2 hw1-2.c

dbg:
	gcc ${FLAGS} -g -D__DEBUG__ -o hw1-1 hw1-1.c
	gcc ${FLAGS} -g -D__DEBUG__ -o hw1-2 hw1-2.c

clean:
	rm -f hw1-1 hw1-2
