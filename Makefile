all: main

main: main.o
	g++ main.o -o main

main.o: main.cpp SudokuSolver.h
	g++ -c main.cpp

clean:
	rm *.o main
