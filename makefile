game: main.o minesweep.o load.o
	gcc -o game main.o minesweep.o load.o

main.o: main.c
	gcc -c main.c

minesweep.o:
	gcc -c minesweep.c

load.o:
	gcc -c load.c

minesweep.c: minesweep.h
load.c: load.h


clean:
	/bin/rm -f *.o game