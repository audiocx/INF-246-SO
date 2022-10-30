all: domino
	./domino

domino: main.o comm.o domino.o
	gcc -o domino main.o comm.o domino.o

main.o: main.c
	gcc -c main.c -Wall

comm.o: comm.c comm.h
	gcc -c comm.c -o comm.o -Wall

domino.o: domino.c domino.h
	gcc -c domino.c -o domino.o -Wall

clean:
	rm -f domino main.o comm.o domino.o