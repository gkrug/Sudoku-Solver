// George Krug
// SudokuSolverSolver.h

// SudokuSolver interface and implementation
// are combined to create a templated class.
// The driver program can instantiate an 
// object of typename int or char. The class
// reads in a SudokuSolver file, and solves it.

#ifndef SudokuSolverSOLVER_H
#define SudokuSolverSOLVER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
using namespace std;

template <typename T>

class SudokuSolver
{
	public:
		SudokuSolver(string);							// non-default constructor
		~SudokuSolver();									// Deconstructor
		void print();											// print function
		void solve();											// Enables user to solve puzzle
		void check_cell(int, int, T);			// Ensures input user choice is valid
		int isValid(int, int, T);					// check valid placement
		void single_block(int, int, T);		// single possibility minigrid check
		int check_block(int, int, T);			// check mini grid for value
		void single_accross(int, int, T);	// Single possibility check accross
		int check_accross(int, int, T);		// check row for value
		void single_down(int, int, T);		// Single possibility check down
		int check_down(int, int, T);			// check column for value
		int check_win();									// Checks if user has solved puzzle
		int check_zero(int, int);					// check for original value
		int single(int, int);							// checks for single possibility
		int size;													// Size is 9
		
	private:
		vector<vector <T> > puzzle;		// 2-d vector array of puzzle
		vector<vector<vector <T> > > possibilities;
		T *givenValues;
		
}; 

// non-default constructor reads in file
template <typename T>
SudokuSolver<T>::SudokuSolver(string SudokuSolver)
{
	size = 9;
	vector<T> tempVec;										// initialize temporary vector
	vector<vector <T> > posstemp;					// 2d temp vec
	ifstream infile;											// input file 
	T tempVar;														// temporary var 
	infile.open (SudokuSolver.c_str());		// open file

	int rowCounter = 0;
	while(!infile.eof()){  								// Push back tempvar in tempvec
  		for(int i = 0; i < size ; i++){    
    		infile >> tempVar; 
      	tempVec.push_back(tempVar); 
   	 }
    puzzle.push_back(tempVec); 					// Push tempvec into 2-d puzzle array
    tempVec.clear(); 										// clear array
	}
	tempVec.clear();

	int counter = 0;
	
	// create array for given value array
	givenValues = new T[size*size];

	// initialize to arbitrary negative value
	for (int a = 0; a < size*size; a++) {
		givenValues[a] = - 1;
	}

	// loop throug puzzle board and record given value cells
	for (int j = 0; j < size; j++) {
		for( int k = 0; k < size; k++) {
			if (puzzle[j][k] != 0) {
				givenValues[counter] = j*9 + k;
				counter++;
			}
		}
	}
	cout << endl;

 // Create 3d possibilities vector
 for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {

				// if not original value, set to 1
				// else, set to 0

				// push back possibility
				if ( check_zero(i, j) )					
					tempVec.push_back(1);				
				else 
					tempVec.push_back(0);

			}
			
			// push back cell's possibilities
			posstemp.push_back(tempVec);		
			tempVec.clear();
		}

		// push back row of cells' with possibilities
		possibilities.push_back(posstemp);
		posstemp.clear();
	}

}

// Default constructor
template <typename T>
SudokuSolver<T>::~SudokuSolver()
{
	delete [] givenValues;
}	

// Function prints out SudokuSolver puzzle 
// in a way that distinguishes minigrids
// and rows and columns
template <typename T>
void SudokuSolver<T>::print()
{
	cout << "    1 2 3  4 5 6  7 8 9" << endl;
	cout << "    * * *  * * *  * * *" << endl;
	for (int i = 0; i < 9; i++) {
		cout << i+1 << " * ";
		for (int j = 0; j < 9; j++) {
			if (j%3 == 0 && j!= 0) 
				cout << "|";
			cout << puzzle[i][j] << " ";
    }
		cout << endl;
		if (i == 2 || i == 5) {
			cout << endl;
		}
	}
	cout << endl;
/*
	for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                        for (int k = 0; k < size; k++) {
                                cout << possibilities[i][j][k] << " ";
                        }
                        cout << endl << i << " " << j << endl;
                }
        }
*/
}

template <typename T>
void SudokuSolver<T>::solve()
{
	T choice;				// possible value
	int row, col;		// cell location
	while (1) {			// loop while the puzzle is unsolved
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				for (T k = 0; k < size; k++) {

					// Call function to eliminate possible cell values
					// and check for single possibility
					check_cell(i, j, k);

					// if single possibility and valid input, enter value in cell
					if ( single( i, j ) && isValid( i, j, k ) ) {
						puzzle[i][j] = k + 1;
						for (int a = 0; a < size; a++) possibilities[i][j][a] = 0;
					}
				}
			}
		}

		// if there are no zeros on the puzzle board, puzzle is solved
		if ( check_win() ) {					
			cout << endl << "********PUZZLE SOLVED*********" << endl;
			print();
			return;
		}
		//print();
	}
}

// call function to check mini-grid, row, and column of choice
template <typename T>
void SudokuSolver<T>::check_cell(int row, int col, T choice)
{
	// if original value return
	if ( check_zero( row, col ) == 0 )
		return;
	else {
		// call functions to eliminate possibilities based on minigrid, rows, and cols
		single_block(row, col, choice);
		single_accross(row, col, choice);
		single_down(row, col, choice);
	}
}

// call function to check mini-grid, row, and column of choice
template <typename T>
int SudokuSolver<T>::isValid(int row, int col, T choice)
{
	// If the value in the cell is valid, return 1
	if ( check_block(row, col, choice) && check_accross(row, col, choice) 
		&& check_down(row, col, choice) )
			return 1;
	else
			return 0;
	
}

// Check mini-grid of user choice of row and col
template <typename T>
void SudokuSolver<T>::single_block(int row, int col, T choice)
{
	int numOnes = 0;
	int tempRow, tempCol;
	int num_given = 0;
	// Divide row and col integer by 3 and scale to get proper mini-grid
	for ( int i = (row/3)*(3); i < (row/3)*(3) + 3; i++) {
		for ( int j = (col/3)*(3); j < (col/3)*(3) + 3; j++) {
			if (choice + 1 == puzzle[i][j]) {
				// If choice equals a value in the minigrid, eliminate cell possibility
				possibilities[row][col][choice] = 0;
			}
			// keep track of number of cell's with the value choice
			if (possibilities[i][j][choice] == 1) {
				++numOnes;
				tempRow = i;
				tempCol = j;
			}
		}
	}
	
	// If there is one cell with a unique possibility and it is a valid entry,
	// enter cell on puzzle board
	if (numOnes == 1 && isValid( tempRow, tempCol, choice) ) {
		puzzle[tempRow][tempCol] = choice + 1;
		// set possibilites for cell to 0
		for (int i = 0; i < size; i++) possibilities[tempRow][tempCol][i] = 0;
	}
		
}

template <typename T>
int SudokuSolver<T>::check_block(int row, int col, T choice)
{
	int numOnes = 0;
	int tempRow, tempCol;
	int num_given = 0;
	// Divide row and col integer by 3 and scale to get proper mini-grid
	for ( int i = (row/3)*(3); i < (row/3)*(3) + 3; i++) {
		for ( int j = (col/3)*(3); j < (col/3)*(3) + 3; j++) {
			if (choice + 1 == puzzle[i][j]) 
				return 0;
		}
	}
	return 1;
}

// Check to make sure user choice is not already in row
template <typename T>
void SudokuSolver<T>::single_accross(int row, int col, T choice)
{
	int numOnes = 0;
	int tempCol;
	int num_given = 0;
	for (int i = 0; i < size; i++) {
		if (choice + 1 == puzzle[row][i]) {
			// if choice value is equal to value in the row, eliminate the possibility
			possibilities[row][col][choice] = 0;
		}
		// track number of cell's with given possibility
		if (possibilities[row][i][choice] == 1) {
				++numOnes;
				tempCol = i;
		}
	}
	// if there is a cell with a unique possibility, fill entry
	if (numOnes == 1 && isValid( row, tempCol, choice ) ) {
		puzzle[row][tempCol] = choice + 1;
		for (int i = 0; i < size; i++) possibilities[row][tempCol][i] = 0;
	}
}

// function checks if value is contained in the row already
template <typename T>
int SudokuSolver<T>::check_accross(int row, int col, T choice)
{
	int numOnes = 0;
	int tempCol;
	int num_given = 0;
	for (int i = 0; i < size; i++) {
		if (choice + 1 == puzzle[row][i]) 
			return 0;
	}
	return 1;
}

// Check to make sure user choice is not already in column
template <typename T>
void SudokuSolver<T>::single_down(int row, int col, T choice)
{
	int numOnes = 0;
	int tempRow;
	int num_given = 0;
	for (int i = 0; i < size; i++) {
		if (choice + 1 == puzzle[i][col]) {
			// Check if value is contained in the column and eliminate possibility if true
			possibilities[row][col][choice] = 0;
		}
		// count cells with that have possible value choice
		if (possibilities[i][col][choice] == 1) {
				++numOnes;
				tempRow = i;
		}
	}
	// if there is a cell with a unique cell possibility, fill entry
	if (numOnes == 1 && isValid( tempRow, col, choice ) ) {
		puzzle[tempRow][col] = choice + 1;
		for (int i = 0; i < size; i++) possibilities[tempRow][col][i] = 0;
	}
}

// Fucntion checks if value is already contained in the column
template <typename T>
int SudokuSolver<T>::check_down(int row, int col, T choice)
{
	int numOnes = 0;
	int tempRow;
	int num_given = 0;
	for (int i = 0; i < size; i++) {
		if (choice + 1 == puzzle[i][col]) 
			return 0;	
	}
	return 1;
}

// If there are no more zeros on puzzle board, puzzle is solved
template <typename T>
int SudokuSolver<T>::check_win()
{
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (puzzle[i][j] == 0)
				return 0;
		}
	}
	return 1;
}

// Before allowing user to modify cell to zero, checks to make sure
// cell is not a given value
template <typename T>
int SudokuSolver<T>::check_zero(int row, int col)
{
	// loop through given value array and check if user row and col
	// choice is a given value
	for (int i = 0; i < 81; i++) {
		if ( (9*row + col) == givenValues[i] ){
			return 0;
		}
	}
	return 1;
}

// Function checks if there is a single possibility for a cell
template <typename T>
int SudokuSolver<T>::single(int row, int column)
{
	int numOnes = 0;
	for (int i = 0; i < size; i++) {
		if (possibilities[row][column][i] == 1)
			++numOnes;
	}
	
	if ( numOnes == 1 )
		return 1;					// single possibility, return true
	else
		return 0;					// else, return 0 false
}

#endif
