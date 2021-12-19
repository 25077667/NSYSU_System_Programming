CC = gcc                                                                  
CFLAGS = -DGEN_DATA -std=gnu11 -Wall -Wextra -Werror -O3
EXE = main.elf
SRC = $(wildcard ./*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
DEP = .dependency

.PHONY: all clean dep

all: $(OBJ)
	$(CC) -o $(EXE) $(OBJ) -lrt -lm

dep:
	$(CC) -MM $(SRC) > .dependency

clean:
	rm -f *.o *.elf $(DEP)
        
%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
