FLAGS=--std=gnu11 -Wall -Wextra -Werror --static

all: dep
	gcc *.o sig_speed_sigsuspend.c ${FLAGS} -o target

hook: dep
	gcc *.o sig_speed_sigsuspend.c -DSIGWAITINFO ${FLAGS} -o target

dep: hook.c error_functions.c get_num.c
	gcc hook.c ${FLAGS} -c
	gcc error_functions.c ${FLAGS} -c
	gcc get_num.c ${FLAGS} -c

clean:
	rm target *.o