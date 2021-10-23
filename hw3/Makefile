FLAGS=-std=gnu11 -Wall -Wextra

all:
	gcc ${FLAGS} part1.c -Ofast -o part1.elf
	strip part1.elf
	gcc ${FLAGS} part2.c -Ofast -o part2.elf
	strip part2.elf
	gcc ${FLAGS} part3.c -Ofast -o part3.elf
	strip part3.elf

dbg:
	gcc ${FLAGS} part1.c -g -o part1.elf
	gcc ${FLAGS} part2.c -g -o part2.elf
	gcc ${FLAGS} part3.c -g -o part3.elf

clean:
	rm -f *.elf