using namespace std;

int l;
int n;
int global_sum;
int* grid;
int** sudoku;
int*** cube;

#include <iostream>
#include <fstream>
#include <limits.h>
// contains updateCell()
#include "update.cpp"
// contains output*()
#include "output.cpp"
// contains check*()
#include "check.cpp"

void test() {
    #pragma omp parallel
    {
        printf("test\n");
    }
}

/* creates 3d cube with dimensions n*n*n and initializes all the values to 1
 */
void createCubeWithOnes() {
    cube = new int**[n];
    for (int i = 0; i < n; i++) {
        cube[i] = new int*[n];
        for (int j = 0; j < n; j++) {
            cube[i][j] = new int[n];
            for (int k = 0; k < n; k++) {
                cube[i][j][k] = 1;
            }
        }
    }
}

/* reads 2d sudoku from "sudoku.txt" in same folder
 */
void readSudoku() {
    ifstream sudokuFile("sudoku.txt", ios::in);

    sudokuFile >> l;
    n = l*l;

    // since n is known, we create the cube here in order to run updateCell()
    createCubeWithOnes();

    sudoku = new int*[n];
    for (int i = 0; i < n; i++) {
        sudoku[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sudokuFile >> sudoku[i][j];

            if(sudoku[i][j] != 0) {
                updateCell(i, j, sudoku[i][j] - 1);
            }
        }
    }
}

/* converts the 3d cube to a 2d sudoku
 * fills in 0 when there is more than one possible value for a cell
 */
void cubeToSudoku() {
    //sum the depths of [i][j][*] whether it is 1
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += cube[i][j][k];
                // check if cell is 1
                if (cube[i][j][k] == 1) {
                    // k + 1 equals the number in the sudoku field, k = 0 ... 8
                    sudoku[i][j] = k + 1;
                }
            }
            // there are multiple possibilities
            if(sum > 1) {
                sudoku[i][j] = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    readSudoku();
    outputSudoku();


    int rating = INT_MAX;
    int previous_rating;
    do {
        previous_rating = rating;
        /* returns the global_sum as rating
         * when the global_sum is unchanged, the loop can stop
         */
        rating = checkCube();
        //cout << rating << " ";
    } while (rating < previous_rating);

    cubeToSudoku();
    outputSudoku();

    return 0;
}
