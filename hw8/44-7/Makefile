FLAGS=--std=gnu11 -Wall -Wextra -Werror --static -Ofast

all: elf
	strip nb*e

elf:
	gcc nbio.c ${FLAGS} -o nbio_ne
	gcc nbio.c -DEXIST ${FLAGS} -o nbio_e
	gcc nbopen.c ${FLAGS} -o nbopen_ne
	gcc nbopen.c -DEXIST ${FLAGS} -o nbopen_e

clean:
	rm -rf nb*e foo