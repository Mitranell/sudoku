/* creates 3d cube with dimensions n*n*n and initializes all the values to 1
 */
void createCubeWithOnes() {
    cube = (int***)malloc(n*sizeof(int**));
    for (int i = 0; i < n; i++) {
        cube[i] = (int**)malloc(n*sizeof(int*));
        for (int j = 0; j < n; j++) {
            cube[i][j] = (int*)malloc(n*sizeof(int));
            for (int k = 0; k < n; k++) {
                cube[i][j][k] = 1;
            }
        }
    }
}

/* reads 2d sudoku from "sudoku.txt" in same folder
 */
void readSudoku() {
    FILE *sudoku_file;
    sudoku_file = fopen("16x16.txt", "r");

    fscanf(sudoku_file, "%d", &l);
    n = l*l;

    // since n is known, we create the cube here in order to run updateCell()
    createCubeWithOnes();

    sudoku = (int**)malloc(n*sizeof(int*));

    for (int i = 0; i < n; i++) {
        sudoku[i] = (int*)malloc(n*sizeof(int));
        for (int j = 0; j < n; j++) {
            fscanf(sudoku_file, "%d", &sudoku[i][j]);

            if(sudoku[i][j] != 0) {
                updateCell(i, j, sudoku[i][j] - 1);
            }
        }
    }

    fclose(sudoku_file);
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

struct cell findEmptyCell(){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += cube[i][j][k];

                if (sum == 2) {
                    return (struct cell){i, j};
                }
            }
        }
    }
}

struct dualCell findEmptyCells(){
    int i1, j1;
    int isSecond = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += cube[i][j][k];

                if (sum == 2) {
                    if (isSecond) {
                        return (struct dualCell){i1, j1, i, j};
                    } else {
                        i1 = i;
                        j1 = j;
                        isSecond = 1;
                    }
                }
            }
        }
    }
}

/* this function times another given function
 */
int timer(int (*function)()) {
    start = clock();
    int result = (*function)();
    // if (result == 0) {
    //      printf("Thread %d: No solution\n\n", thread_rank);
    // }
    duration = (clock() - start) / (double) CLOCKS_PER_SEC;
    return result;
}

void MPI_check(){
    // check for messages
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &iprobe_flag, &status);

    // there is a message
    if (iprobe_flag) {
        // reading message
        MPI_Recv(&data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        // a thread confirms to have received the response
        if (status.MPI_TAG == 0) {
            //TODO: set quit_node
        }
        // a thread asks for search space
        else {
            // let the source know that we have read it
            // TODO: set buffer to sharing node
            buffer = 0;
            MPI_Isend(&buffer, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &request);
        }
    }
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
        return 1;
    }

    // the sudoku is not solvable, so trigger backtracking
    if (rating == 0) {
        return 0;
    }

    struct cell backtrackCell = findEmptyCell();
    int i = backtrackCell.i;
    int j = backtrackCell.j;

    /* the sudoku is not solved yet
     * we try a value and recursively call the same function
     */
    for (int k = 0; k < n; k++) {
        MPI_check();
        if (cube[i][j][k]) {
            int temp_cube[n][n][n];
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    for (int k = 0; k < n; k++) {
                        temp_cube[i][j][k] = cube[i][j][k];
                    }
                }
            }

            updateCell(i, j, k);

            if (solve()) {
                return 1;
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    for (int k = 0; k < n; k++) {
                        cube[i][j][k] = temp_cube[i][j][k];
                    }
                }
            }
        }
    }

    // non of the values are possible, so trigger backtracking
    return 0;
}
