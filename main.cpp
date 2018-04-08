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

/* this function times another given function
 */
int timer(int (*function)()) {
    start = std::clock();
    int result = (*function)();
    if (result == 0) {
        printf("Not solution")
    }
    //duration = duration + (std::clock() - start) / (double) CLOCKS_PER_SEC;
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    //cout << "Duration: "<< duration << endl << endl;
    return result;
}

int count = 0;

int solve() {
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

    /* the sudoku is not solved yet
     * we try a value and recursively call the same function
     */
    for (int k = 0; k < n; k++) {
        if (cube[backtrack.i][backtrack.j][k]) {
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

            updateCell(backtrack.i, backtrack.j, k);

            if (solve()) {
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
