/* for each column and possible cell value: check all rows for uniqueness
 */
int checkRow() {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            int p;

            // loop over the whole row
            for (int i = 0; i < n; i++) {
                sum += cubes[v][i][j][k];
                if (cubes[v][i][j][k]) {
                    p = i;
                }
            }

            if (sum == 1) {
                updateCell(p, j, k);
            }

            total_sum += sum;
        }
    }
    return 1;
}

/* for each row and possible cell value: check all columns for uniqueness
 */
int checkColumn() {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            int sum = 0;
            int p;

            // loop over the whole column
            for (int j = 0; j < n; j++) {
                sum += cubes[v][i][j][k];
                //save the right column
                if (cubes[v][i][j][k]) {
                    p = j;
                }
            }

            // if the sum equals 0, there is no possibility
            if (sum == 0) {
                return 0;
            }

            // if the sum equals 1, then it was the only one
            if (sum == 1) {
                updateCell(i, p, k);
            }

            total_sum += sum;
        }
    }
    return 1;
}

/* for each row and column: check all possible cell values for uniqueness
 */
int checkCell() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            int p;

            // loop over all the cell its possible values
            for (int k = 0; k < n; k++) {
                sum += cubes[v][i][j][k];
                // save the right depth
                if (cubes[v][i][j][k]) {
                    p = k;
                }
            }

            // if the sum equals 0, there is no possibility
            if (sum == 0) {
                return 0;
            }

            // if the sum equals 1, then it was the only one
            if (sum == 1) {
                updateCell(i,j,p);
            }

            total_sum += sum;
        }
    }
    return 1;
}

/* for each possible cell value: check all grids for uniqueness
 */
int checkGrid() {
    for (int k = 0; k < n; k++) {
        int p;
        int q;

        // increase indices with size of grid
        for (int i = 0; i < n; i += l) {
            for (int j = 0; j < n; j += l) {
                int sum = 0;
                grid = getGrid(i, j);

                // loop over the grid
                for (int x = grid[0]; x <= grid[2]; x++) {
                    for (int y = grid[1]; y <= grid[3]; y++) {
                        sum += cubes[v][x][y][k];
                        // save the right cell
                        if (cubes[v][x][y][k]) {
                            p = x;
                            q = y;
                        }
                    }
                }

                // if the sum equals 0, there is no possibility
                if (sum == 0) {
                    return 0;
                }

                // if the sum equals 1, then it was the only one
                if (sum == 1) {
                    updateCell(p,q,k);
                }

                total_sum += sum;
            }
        }
    }
    return 1;
}

/* check the rows, columns, possible cell values and grids for uniqueness
 * if there is only one possibility for some cell due to these checks
 * then the value for this cell is updated
 * if there is a row, column or grid without a possibility, it will return 0
 */
int checkCube() {
    total_sum = 0;
    if(checkRow() && checkColumn() && checkCell() && checkGrid()) {
        return total_sum;
    } else {
        return 0;
    }
}

// /* Check if sudoku is solved
// */
// bool isSolved(){
//     for (int k = 0; k < n; k++) {
//         for (int j = 0; j < n; j++) {
//             int sum = 0;
//             int p;
//
//             // loop over the whole row
//             for (int i = 0; i < n; i++) {
//                 sum += cubes[v][i][j][k];
//                 if (cubes[v][i][j][k]) {
//                     p = i;
//                 }
//             }
//
//             if (sum != 1) {
//                 return false;
//             }
//         }
//     }
//     return true;
// }
//
// /* Check if cube just zeros => wrong didgit in bruteforcing
// */
// bool isSolvable(){
//         for (int k = 0; k < n; k++) {
//         for (int j = 0; j < n; j++) {
//             int sum = 0;
//             int p;
//
//             // loop over the whole row
//             for (int i = 0; i < n; i++) {
//                 sum += cubes[v][i][j][k];
//                 if (cubes[v][i][j][k]) {
//                     p = i;
//                 }
//             }
//
//             if (sum == 0) {
//                 return false;
//             }
//         }
//     }
//     return true;
// }
