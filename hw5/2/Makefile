FLAGS=-lstdc++ -static -Wall 
all: popen.o
	gcc *.o driver.c ${FLAGS} -o target

popen.o: popen.[ch]
	gcc popen.[ch] ${FLAGS} -g -c

# format_cmd.o: fmtcmd.[ch]pp
# 	g++ fmtcmd.cpp ${FLAGS} -c

clean:
	rm target popen.h.gch popen.o