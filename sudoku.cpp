using namespace std;

#include <vector>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>

typedef vector<int> v1d;
typedef vector<v1d> v2d;
typedef vector<v2d> v3d;
typedef vector<v3d> v4d;

int l;
int n;
int total_sum;
int* grid;
v4d cubes;
int v = 0;
struct flag_struct {
    int i,j,k;
};
vector<flag_struct> v_flag;
// TODO: comment
flag_struct intial = {-1,-1,-1};
clock_t start;
double duration;

#include "update.cpp"
#include "output.cpp"
#include "check.cpp"
#include "bruteforce.cpp"

/* create a three dimesional vector of size n-n-n filled with only ones
 */
void createCubeWithOnes() {
    v1d row(n, 1);
    v2d plane(n, row);
    v3d cube(n,plane);

    cubes.push_back(cube);
}

/* create an two dimensional array of size n-n
 */
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
                if (cubes[v][i][j][k] == 1) {
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

/* this function times another given function
 */
void timer(void (*function)()) {
    start = std::clock();
    (*function)();
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout<< "Duration: "<< duration << endl << endl;
}

void solve() {
    int rating = INT_MAX;
    int previous_rating;

    //START not tested, might be not correct
    do {
        do {
            cout << "rating: " << rating << " previous rating: " << previous_rating << endl;
            previous_rating = rating;
            // returns the total_sum as rating
            rating = checkCube();
            // when the total_sum is unchanged, the loop can stop
        } while (0 != rating && rating < previous_rating);
        break;
        if (rating == 0) {
            // bruteforce was wrong, revert it
            revertBruteforceStep();
            previous_rating = INT_MAX;
        }
        if (rating != 4*n*n) {
            bruteforce();
            //outputDepth(0,0);
        }
    } while(rating != 4*n*n);
    //END not tested, might be not correct

    // cout << "rating: " << rating << endl;
    // cout << "isSolved: " << isSolved() << endl;
}

int main(int argc, char *argv[]) {
    // TODO: comment
    v_flag.push_back(intial);

    outputSudoku(readSudoku());
    timer(&solve);
    outputSudoku(cubeToSudoku());

    return 0;
}
