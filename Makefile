matMultp: matMultp.o matrix.o
	gcc -pthread matMultp.o matrix.o -o matMultp

matMultp.o: matMultp.c matrix.h
	gcc -c matMultp.c

matrix.o : matrix.c matrix.h
	gcc -c matrix.c