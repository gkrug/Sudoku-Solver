// George Krug
// main.cpp
// The driver program instantiates and object of 
// type int and inputs a file name, "sudoku.txt"
// the play function is called allowing the user 
// to attempt to solve the puzzle

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "SudokuSolver.h"

using namespace std;

int main()
{

	// instantiate object of ints
	SudokuSolver<int> puzzle1("easy.txt");
	cout << endl << "    *******Easy******" << endl;
	puzzle1.print();
	puzzle1.solve();
	
	SudokuSolver<int> puzzle2("medium.txt");
	cout << endl << "    *******Medium*******" << endl;
	puzzle2.print();
	puzzle2.solve();

/*
	SudokuSolver<int> puzzle3("hard.txt");
	cout << endl << "    ******Hard*****" << endl;
	puzzle3.print();
	puzzle3.solve();
*/

}
