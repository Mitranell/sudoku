/* for each column and possible cell value: check all rows for uniqueness
 */
void checkRow() {
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            int p;

            // loop over the whole row
            for (int i = 0; i < n; i++) {
                sum += cube[i][j][k];
                if (cube[i][j][k]) {
                    p = i;
                }
            }

            if (sum == 1) {
                updateCell(p, j, k);
            }

            global_sum += sum;
        }
    }
}

/* for each row and possible cell value: check all columns for uniqueness
 */
void checkColumn() {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            int sum = 0;
            int p;

            // loop over the whole column
            for (int j = 0; j < n; j++) {
                sum += cube[i][j][k];
                //save the right column
                if (cube[i][j][k]) {
                    p = j;
                }
            }

            // if the sum equals 1, then it was the only one
            if (sum == 1) {
                updateCell(i, p, k);
            }

            global_sum += sum;
        }
    }
}

/* for each row and column: check all possible cell values for uniqueness
 */
void checkCell() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            int p;

            // loop over all the cell its possible values
            for (int k = 0; k < n; k++) {
                sum += cube[i][j][k];
                // save the right depth
                if (cube[i][j][k]) {
                    p = k;
                }
            }

            // if the sum equals 1, then it was the only one
            if (sum == 1) {
                updateCell(i,j,p);
            }

            global_sum += sum;
        }
    }
}

/* for each possible cell value: check all grids for uniqueness
 */
void checkGrid() {
    for (int k = 0; k < n; k++) {
        int sum = 0;
        int p;
        int q;

        // increase indices with size of grid
        for (int i = 0; i < n; i += l) {
            for (int j = 0; j < n; j += l) {
                grid = getGrid(i, j);

                // loop over the grid
                for (int x = grid[0]; x <= grid[2]; x++) {
                    for (int y = grid[1]; y <= grid[3]; y++) {
                        sum += cube[x][y][k];
                        // save the right cell
                        if (cube[x][y][k]) {
                            p = x;
                            q = y;
                        }
                    }
                }

                // if the sum equals 1, then it was the only one
                if (sum == 1) {
                    updateCell(p,q,k);
                }

                global_sum += sum;
            }
        }
    }
}

/* check the rows, columns, possible cell values and grids for uniqueness
 * if there is only one possibility for some cell due to these checks
 * then the value for this cell is updated
 */
int checkCube() {
    global_sum = 0;
    checkRow();
    checkColumn();
    checkCell();
    checkGrid();
    return global_sum;
}
