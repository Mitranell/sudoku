#include <iostream>
#include <fstream>


using namespace std;
void test() {
#pragma omp parallel
	{
		printf("test\n");
	}
}

void outputSudoku(int** array, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << array[i][j] << " ";
		}
		cout << endl;
	}
}
void outputRow(int*** cube, int i, int k, int n) {
	for (int j = 0; j < n; j++) {
		cout << cube[i][j][k] << " " ;
	}
	cout << endl;
}
void outputColumn(int*** cube, int j, int k, int n) {
	for (int i = 0; i < n; i++) {
		cout << cube[i][j][k] << endl;
	}
}
void outputDepth(int*** cube, int i, int j, int n) {
	for (int k = 0; k < n; k++) {
		cout << cube[i][j][k] << " ";
	}
	cout << endl;
}
void outputSliceCube(int*** cube, int k, int n) {
	for (int x = 0; x < n; x++) {
		outputRow(cube, x, k, n);
	}
}
int** cubeToSudoku(int*** cube, int l) {
	int n = l*l;
	int** array = new int*[n];
	for (int i = 0; i < n; i++) {
		array[i] = new int[n];
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int sum = 0;
			for (int k = 0; k < n; k++) {
				sum += cube[i][j][k];
				if (cube[i][j][k] == 1) {
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

int* getGrid(int i, int j, int l) {
	int i1 = (i / l) * l;
	int j1 = (j / l) * l;
	int i2 = i1 + l-1;
	int j2 = j1 + l-1;
	int grid[] = { i1, j1, i2, j2};

	return grid;
}

void setColumn(int*** cube, int i, int j, int k, int n) {
	for (int x = 0; x < n; x++) {
		if (i != x) {
			cube[x][j][k] = 0;
		}
	}
}

void setRow(int*** cube, int i, int j, int k, int n) {
	for (int x = 0; x < n; x++) {
		if (j != x) {
			cube[i][x][k] = 0;
		}
	}
}

void setDepth(int*** cube, int i, int j, int k, int n) {
	for (int x = 0; x < n; x++) {
		if (k != x) {
			cube[i][j][x] = 0;
		}
	}
}

void setGrid(int*** cube, int i, int j, int k, int l) {
	int* grid = getGrid(i, j, l);

	for (int x = grid[0]; x <= grid[2]; x++) {
		for (int y = grid[1]; y <= grid[3]; y++) {
			if (i != x && j != y) {
				cube[x][y][k] = 0;
			}
		}
	}
}

void updateCell(int*** cube, int i, int j, int k, int l) {
	// [i][j][k] is supposed to be 1 
	int n = l*l;
	setRow(cube, i, j, k, n);
	setColumn(cube, i, j, k, n);
	setDepth(cube, i, j, k, n);
	setGrid(cube, i, j, k, l);
}

void initCube(int** sudoku, int*** cube, int l) {
	int n = l*l;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (sudoku[i][j] != 0) {
				int k = sudoku[i][j] - 1;
				updateCell(cube, i, j, k, l);
			}
		}
	}
}

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
				if (cube[i][j][k]) {
					m = k;
				}
			}
			global_sum += sum;
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
	int foo = checkRowColumnDepth(cube, l);
	cout << foo << endl;
	while (foo > (foo = checkRowColumnDepth(cube, l))) {
		cout << foo << endl;
	}
		

	//for (int k = 0; k < n; k++) {
	//	outputSliceCube(cube, k, n);
	//	cout << endl;
	//}
	int** sudokuOutput = cubeToSudoku(cube, l);
	if (NULL != sudokuOutput) {
		outputSudoku(sudokuOutput, l*l);
	}
	else {
		cout << "not solvable for me.";
	}
	

	return 0;
}


