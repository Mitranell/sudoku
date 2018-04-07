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
    ifstream sudoku_file("9x9.txt", ios::in);

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

/* this function times another given function
 */
void timer(int (*function)()) {
    start = std::clock();
    if ((*function)()) {
        cout << "Solved!" << endl;
    } else {
        cout << "Not solved!" << endl;
    }
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "Duration: "<< duration << endl << endl;
}

int solve() {
    int rating = INT_MAX;
    int previous_rating;

    do {
        previous_rating = rating;
        rating = checkCube();
    } while (0 != rating && rating < previous_rating);

    // the sudoku is solved
    if (rating == 4*n*n) {
        cout << "rating = solved" << endl;
        return 1;
    }

    // the sudoku is not solvable, so trigger backtracking
    if (rating == 0) {
        cout << "rating = 0" << endl;
        return 0;
    }

    /* the sudoku is not solved yet
     * we try a value and recursively call the same function
     */
    for (int k = 0; k < n; k++) {
        if (cube[backtrack.i][backtrack.j][k]) {
            int*** temp_cube = cube;
            updateCell(backtrack.i, backtrack.j, k);
            cube = temp_cube;

            if (solve()) {
                return 1;
            }

            cube = temp_cube;
        }
    }

    // non of the values are possible, so trigger backtracking
    return 0;
}
