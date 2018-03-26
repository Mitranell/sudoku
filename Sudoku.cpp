#include <iostream>
#include <fstream>
#include <limits.h>


using namespace std;
void test() {
#pragma omp parallel
	{
		printf("test\n");
	}
}
 /* takes the 2d sudoku field and dimension @param n
 *  as input and displays all field
 */
void outputSudoku(int** array, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}

 /* displays the row [i][ ][k] of the 3d cube
 */
void outputRow(int*** cube, int i, int k, int n) {
	for (int j = 0; j < n; j++) {
		cout << cube[i][j][k] << " " ;
	}
	cout << endl;
}

 /* dispays the column [ ][j][k] of the 3d cube
 */
void outputColumn(int*** cube, int j, int k, int n) {
	for (int i = 0; i < n; i++) {
		cout << cube[i][j][k] << endl;
	}
}

/* dispays the depth [i][j][ ] of the 3d cube
*/
void outputDepth(int*** cube, int i, int j, int n) {
	for (int k = 0; k < n; k++) {
		cout << cube[i][j][k] << " ";
	}
	cout << endl;
}

/* dispays all the rows and columns as a 2d  planar of the depth
 * [ ][ ][k] of the 3d cube
*/
void outputSliceCube(int*** cube, int k, int n) {
	for (int x = 0; x < n; x++) {
		outputRow(cube, x, k, n);
	}
}

 /* tries to convert the 3d cube back to 2d sudoku form
  * by checking in which field a 1 appears, error if sum per
  * depth is bigger than 1, in case of error NULL
  * will be returned
 */
int** cubeToSudoku(int*** cube, int l) {
	int n = l*l;
	//create new 2d array for sudoku
	int** array = new int*[n];
	for (int i = 0; i < n; i++) {
		array[i] = new int[n];
	}
	//sum the depths of [i][j][ ] whether it is 1
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int sum = 0;
			for (int k = 0; k < n; k++) {
				sum += cube[i][j][k];
				//check if cell is 1
				if (cube[i][j][k] == 1) {
					// k+1 equals the number and the sudoku field, k=0,..8
					array[i][j] = k + 1;
				}
			}
			if (sum != 1) {
				return NULL;
			}
		}
	}
	return array;
}

 /* reads 2d sudoku from file where the file is in the same folder
  * and named as "sudoku.txt" and returns the sudoku array
  */
int** readSudoku(int l) {
	int n = l*l;
	int** array = new int*[n];
	for (int i = 0; i < n; i++) {
		array[i] = new int[n];
	}

	ifstream sudoku("sudoku.txt", ios::in);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			sudoku >> array[i][j];
		}
	}
	return array;
}

 /* creates the 3d cube with dimensions n*n*n and initializes
  * all the values with 1.
  */
int*** createCubeWithOnes(int n) {
	int*** cube= new int**[n];
	for (int i = 0; i < n; i++) {
		cube[i] = new int*[n];
		for (int j = 0; j < n; j++) {
			cube[i][j] = new int[n];
			for (int k = 0; k < n; k++) {
				cube[i][j][k] = 1;
			}
		}
	}
	return cube;
}
 /* gets for any cell in the form [i][j] the corresponding grid in the form
  * of an array; [x1, y1, x2, y2] where (x1,y1) shows the starting point
  * and (x2,y2) the end point that spreads up the cell
  */
int* getGrid(int i, int j, int l) {
	// l gives size of one grid: l*l
	// integer calculation, cuts off the after comma values, always gives the beginning of the grid
	int i1 = (i / l) * l;
	int j1 = (j / l) * l;
	// last field which is still in the grid is l-1 fields in x and y direction
	int i2 = i1 + l-1;
	int j2 = j1 + l-1;
	int grid[] = {i1, j1, i2, j2};
	int * gridPointer = grid;

	return gridPointer;
}

 /* given the cell [i][j][k] sets all the cells in the same column to zero
  * except the given cell
  */
void setColumn(int*** cube, int i, int j, int k, int n) {
	for (int x = 0; x < n; x++) {
		if (i != x) {
			//iterates over the rows the get the whole column
			cube[x][j][k] = 0;
		}
	}
}

 /* given the cell [i][j][k] sets all the cells in the same row to zero
  * except the given cell
  */
void setRow(int*** cube, int i, int j, int k, int n) {
	for (int x = 0; x < n; x++) {
		if (j != x) {
			//iterates over the columns the get the whole row
			cube[i][x][k] = 0;
		}
	}
}
 /* given the cell [i][j][k] sets all the cells in the same depth to zero
  * except the given cell
  */
void setDepth(int*** cube, int i, int j, int k, int n) {
	for (int x = 0; x < n; x++) {
		if (k != x) {
			//iterates over the depths the get the whole depth
			cube[i][j][x] = 0;
		}
	}
}

 /* takes the cell [i][j][k] and sets all the cells in the same grid to zero
  * except the given cell
  */
void setGrid(int*** cube, int i, int j, int k, int l) {
	int* grid = getGrid(i, j, l);

	//loop over the whole grid, set everything 0 except when the indices align with the given cell
	for (int x = grid[0]; x <= grid[2]; x++) {
		for (int y = grid[1]; y <= grid[3]; y++) {
			if (i != x && j != y) {
				cube[x][y][k] = 0;
			}
		}
	}
}

 /* combines the previous commands as one, with a given cell, sets the rest of the row/column/depth/grid to zero*/
void updateCell(int*** cube, int i, int j, int k, int l) {
	// [i][j][k] is supposed to be 1
	int n = l*l;
	setRow(cube, i, j, k, n);
	setColumn(cube, i, j, k, n);
	setDepth(cube, i, j, k, n);
	setGrid(cube, i, j, k, l);
}

 /* takes the 2d sudoku as input and according to the values sets the values of the cube to zero*/
void initCube(int** sudoku, int*** cube, int l) {
	int n = l*l;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			// check if given field is empty
			if (sudoku[i][j] != 0) {
				//dimension k aligns with number of the cell - 1
				int k = sudoku[i][j] - 1;
				updateCell(cube, i, j, k, l);
			}
		}
	}
}

 /* takes the cube and loops over row/column/depth to check if sum equals one for each, if thats
  * the case the cell with the 1 has the right value and other cells in row/column/depth/grid will be set to zero.
  * indicator to stop the loop forever is a sum over all rows/columns/depths, if this number doesnt decrease we
  * can stop
  */
int checkRowColumnDepth(int*** cube, int l) {
	int n = l*l;
	int global_sum = 0;
	//depth sum
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++){
			int sum = 0;
			int m = 0;
			for (int k = 0; k < n; k++) {
				sum += cube[i][j][k];
				//save which depth cell has the 1 to updates the other cells
				if (cube[i][j][k]) {
					m = k;
				}
			}
			global_sum += sum;
			//if sum isnt 1 we cant be sure that the cell contains the right value
			if (sum == 1) {
				updateCell(cube,i,j,m,l);
			}
		}
	}

	//column sum
	for (int i = 0; i < n; i++) {
		for (int k = 0; k < n; k++) {
			int sum = 0;
			int m = 0;
			for (int j = 0; j < n; j++) {
				sum += cube[i][j][k];
				//save the right column
				if (cube[i][j][k]) {
					m = j;
				}
			}
			global_sum += sum;
			if (sum == 1) {
				updateCell(cube, i, m, k, l);
			}
		}
	}

	//row sum
	for (int k = 0; k < n; k++) {
		for (int j = 0; j < n; j++) {
			int sum = 0;
			int m = 0;
			for (int i = 0; i < n; i++) {
				sum += cube[i][j][k];
				if (cube[i][j][k]) {
					m = i;
				}
			}
			global_sum += sum;
			if (sum == 1) {
				updateCell(cube, m, j, k, l);
			}
		}
	}

	return global_sum;
}

int main(int argc, char *argv[]) {
	//test();
	//for (int i = 0; i < argc; i++) {
	//	printf(argv[i]);
	//	printf("\n");
	//}

	//if (argc < 2) {
	//	return 0;
	//}

	//read l
	//char lchar = *argv[1];
	//int l = lchar - 48;
	//cout << l;

	// l is set by hand for easier develpoment, needs to be adjusted according to size of input
	int l = 3;
	int n = l*l;
	int** sudoku = readSudoku(l);
	outputSudoku(sudoku, n);
	cout << endl;
	int*** cube = createCubeWithOnes(n);
	//setRow(cube, 0, 0, 0, n);
	//setColumn(cube, 0, 0, 0, n);
	//setDepth(cube, 0, 0, 0, n);

	//outputRow(cube, 0, 0, n);
	//outputColumn(cube, 0, 0, n);
	//outputDepth(cube, 0, 0, n);
	initCube(sudoku, cube, l);

	//checkRowColumnDepth returns the global_sum as rating which can be used to not let it loop forever
	int rating = INT_MAX;
	int previous_rating;
	do {
		previous_rating = rating;
		rating = checkRowColumnDepth(cube, l);
		cout << rating << endl;
	} while (rating < previous_rating);




	//for (int k = 0; k < n; k++) {
	//	outputSliceCube(cube, k, n);
	//	cout << endl;
	//}
	int** sudokuOutput = cubeToSudoku(cube, l);
	if (NULL != sudokuOutput) {
		outputSudoku(sudokuOutput, l*l);
	}
	else {
		cout << "not solvable for me.\n";
	}


	return 0;
}
