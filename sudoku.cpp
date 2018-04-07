using namespace std;

int l;
int n;
int total_sum;
int* grid;
int*** cube;

#include <iostream>
#include <fstream>
#include <limits.h>
#include <time.h>
// #include <sys/time.h>
#include <omp.h>
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
// void createCubeWithOnes() {
//     cube = new int**[n];
//     for (int i = 0; i < n; i++) {
//         cube[i] = new int*[n];
//         for (int j = 0; j < n; j++) {
//             cube[i][j] = new int[n];
//             for (int k = 0; k < n; k++) {
//                 cube[i][j][k] = 1;
//             }
//         }
//     }
// }

// PARALLEL
void createCubeWithOnes() {
    // creating as many threads as there are rows in the sudoku
    omp_set_num_threads(n);

    cube = new int**[n];

    #pragma omp parallel for
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

int** init_sudoku() {
    int** sudoku = new int*[n];
    for (int i = 0; i < n; i++) {
        sudoku[i] = new int[n];
    }

    return sudoku;
}

/* reads 2d sudoku from "sudoku.txt" in same folder
 */
int** readSudoku() {
    ifstream sudoku_file("16x16.txt", ios::in);

    sudoku_file >> l;
    n = l*l;

    // since n is known, we create the cube here in order to run updateCell()
    createCubeWithOnes();

    int** sudoku = init_sudoku();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sudoku_file >> sudoku[i][j];

            if(sudoku[i][j] != 0) {
                updateCell(i, j, sudoku[i][j] - 1);
            }
        }
    }
    return sudoku;
}

/* converts the 3d cube to a 2d sudoku
 * fills in 0 when there is more than one possible value for a cell
 */
int** cubeToSudoku() {
    int** sudoku = init_sudoku();

    //sum the depths of [i][j][*] whether it is 1
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                // check if cell is 1
                if (cube[i][j][k] == 1) {
                    if (sum == 0) {
                        sum = 1;
                        /* k + 1 equals the number in the sudoku field
                         * k = 0 ... 8
                         */
                        sudoku[i][j] = k + 1;
                    } else {
                        // there are multiple possibilities
                        sudoku[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }

    return sudoku;
}

void solve() {
    int rating = INT_MAX;
    int previous_rating;
    do {
        previous_rating = rating;
        // returns the total_sum as rating
        rating = checkCube();
        // when the total_sum is unchanged, the loop can stop
    } while (rating < previous_rating);
}


int main(int argc, char *argv[]) {
    
    outputSudoku(readSudoku());
    
    // initializing timer 
    clock_t timeStamp;
    timeStamp = clock();

    solve();

    timeStamp = clock() - timeStamp;

    double time_taken = ((double)timeStamp)/CLOCKS_PER_SEC; // in seconds
 
    outputSudoku(cubeToSudoku());

    printf("solve() took %f seconds to execute \n", time_taken); 

    return 0;
}
