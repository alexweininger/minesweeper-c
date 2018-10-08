game: main.o minesweep.o load.o
	gcc -g -o minesweep main.o minesweep.o load.o

main.o: main.c
	gcc -g -c main.c

minesweep.o: minesweep.c minesweep.h
	gcc -g -c minesweep.c

load.o: load.c load.h
	gcc -g -c load.c

clean:
	/bin/rm -f *.o minesweep