FLAGS=-Wall -Wextra -Werror
all: popen.o fmtcmd.o
	gcc *.o driver.c ${FLAGS} -static -lstdc++ -Ofast -o target
	strip target

popen.o: dict.o popen.[ch] 
	gcc popen.[ch] ${FLAGS} -c

fmtcmd.o: fmtcmd.cpp
	g++ fmtcmd.cpp ${FLAGS} -Ofast -c

dict.o: dict.cpp
	g++ dict.cpp ${FLAGS} -Ofast -std=c++17 -c

clean:
	rm target popen.h.gch popen.o