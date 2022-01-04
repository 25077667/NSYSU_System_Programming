FLAGS=--std=gnu11 -Wall -Wextra -Werror
O=main.elf

all: my_alarm.o main.c
	gcc main.c my_alarm.o ${FLAGS} -o $(O)
	strip $(O)

my_alarm.o: my_alarm.c
	gcc my_alarm.c ${FLAGS} -c

clean:
	rm $(O) *.o