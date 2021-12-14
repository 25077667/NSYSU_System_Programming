FLAGS=--std=gnu11 -Wall -Wextra -Werror -Ofast

all: reader writer
	strip reader 
	strip writer

reader: dep svshm_xfr_reader.c
	gcc *.o svshm_xfr_reader.c -o reader ${FLAGS} -lrt -lpthread

writer: dep svshm_xfr_writer.c
	gcc *.o svshm_xfr_writer.c -o writer ${FLAGS} -lrt -lpthread

dep: get_num.o error_functions.o binary_sems.o  

get_num.o: get_num.c
	gcc get_num.c ${FLAGS} -c

error_functions.o: error_functions.c
	gcc error_functions.c ${FLAGS} -c

binary_sems.o: binary_sems.c
	gcc binary_sems.c ${FLAGS} -c

clean:
	rm reader writer *.o