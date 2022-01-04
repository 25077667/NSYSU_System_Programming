CC = gcc                                                                  
CFLAGS = -std=c11
EXE = main.elf
SRC = $(wildcard ./*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
DEP = .dependency

.PHONY: all clean dep 

all: $(OBJ)
	$(CC) -o $(EXE) $(OBJ)

dep:
	$(CC) -MM $(SRC) > .dependency

clean:
	rm -f *.o *.elf $(DEP)
        
%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<
