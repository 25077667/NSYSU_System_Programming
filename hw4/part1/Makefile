FLAGS= -Wall -Wextra -std=gnu11

all: compile
	ln busybox hostinfo
	ln busybox mydate
	ln busybox printdir
	ln busybox mycat
	ln busybox pipe_ls

compile: 
	gcc ${FLAGS} -Ofast --static lib.[ch] main.c -o busybox
	strip busybox

dbg: 
	gcc -g lib.[ch] main.c -o busybox

clean:
	rm busybox hostinfo mydate printdir mycat pipe_ls