int once = 1;

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

/* create an two dimensional array of size n-n
 */
// int** init_sudoku() {
//     int** sudoku = new int*[n];
//     for (int i = 0; i < n; i++) {
//         sudoku[i] = new int[n];
//     }
//
//     return sudoku;
// }

/* reads 2d sudoku from "sudoku.txt" in same folder
 */
void readSudoku() {
    ifstream sudoku_file("16x16.txt", ios::in);

    sudoku_file >> l;
    n = l*l;

    // since n is known, we create the cube here in order to run updateCell()
    createCubeWithOnes();

    sudoku = new int*[n];
    for (int i = 0; i < n; i++) {
        sudoku[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sudoku_file >> sudoku[i][j];

            if(sudoku[i][j] != 0) {
                updateCell(i, j, sudoku[i][j] - 1);
            }
        }
    }
}

/* converts the 3d cube to a 2d sudoku
 * fills in 0 when there is more than one possible value for a cell
 */
int** cubeToSudoku() {
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
}

cell findEmptyCell(){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += cube[i][j][k];

                if (sum == 2) {
                    return {i, j};
                }
            }
        }
    }
}

/* this function times another given function
 */
int timer(int (*function)()) {
    start = std::clock();
    int result = (*function)();
    if (result == 0) {
         cout << "No solution" << endl;
    }
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    return result;
}
int counter = 0;

int solve(int rank) {


    int rating = INT_MAX;
    int previous_rating;

    do {
        previous_rating = rating;
        //rating = timer(&checkCube);
        rating = checkCube();
    } while (0 != rating && rating < previous_rating);


    // the sudoku is solved
    if (rating == 4*n*n) {
        return 1;
    }

    // the sudoku is not solvable, so trigger backtracking
    if (rating == 0) {
        return 0;
    }

    if(once) {
        cell backtrackCell = findEmptyCell();
        updateCell(backtracCell.i, backtrackCell.j, rank + 1);

        once = 0;
    }

    cell backtrackCell = findEmptyCell();
    int i = backtrackCell.i;
    int j = backtrackCell.j;

    /* the sudoku is not solved yet
     * we try a value and recursively call the same function
     */
    for (int k = 0; k < n; k++) {
        onFirstRound = false;
        if (cube[i][j][k]) {
            int*** temp_cube = new int**[n];
            for (int i = 0; i < n; i++) {
                temp_cube[i] = new int*[n];
                for (int j = 0; j < n; j++) {
                    temp_cube[i][j] = new int[n];
                    for (int k = 0; k < n; k++) {
                        temp_cube[i][j][k] = cube[i][j][k];
                    }
                }
            }


            updateCell(i, j, k);

            if (solve(rank)) {
                return 1;
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    for (int k = 0; k < n; k++) {
                        cube[i][j][k] = temp_cube[i][j][k];
                    }
                    delete[] temp_cube[i][j];
                }
                delete[] temp_cube[i];
            }
            delete[] temp_cube;
        }
    }

    // non of the values are possible, so trigger backtracking
    return 0;
}
