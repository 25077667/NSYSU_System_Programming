FLAGS=--std=gnu11 -Wall -Wextra -Werror -lpthread -g
O=main.elf

all: main.c reader.o writer.o
	gcc main.c *.o ${FLAGS} -o $(O)
	# strip $(O)

reader.o: reader.c
	gcc reader.c ${FLAGS} -c

writer.o: writer.c
	gcc writer.c ${FLAGS} -c

clean:
	rm $(O) *.o