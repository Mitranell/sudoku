/* gets the coordinates [x1, y1, x2, y2] of the grid where cell [i][j] lies in
 * (x1,y1) upper left cell and (x2,y2) the lower right cell
 */
int* getGrid(int i, int j) {
    // integer calculation cuts off the fractional part
    int i1 = (i / l) * l;
    int j1 = (j / l) * l;
    // last field which is still in the grid are the l-1 fields
    int i2 = i1 + l - 1;
    int j2 = j1 + l - 1;
    int grid[] = {i1, j1, i2, j2};
    int * gridPointer = grid;

    return gridPointer;
}

/* takes the cell [i][j][k] and sets all the cells in the same grid to zero
 * except the given cell
 */
void setGrid(int i, int j, int k) {
    grid = getGrid(i, j);

    /* loop over the whole grid
     * set everything to 0 except when the indices align with the given cell
     */
    for (int x = grid[0]; x <= grid[2]; x++) {
        for (int y = grid[1]; y <= grid[3]; y++) {
            if (i != x && j != y) {
                cubes[v][x][y][k] = 0;
            }
        }
    }
}

/* given the cell [i][j][k] sets all the cells in the same bar in some direction
 * to zero, except the given cell
 */
void setBar(int *i, int *j, int *k, int *direction) {
    int original = *direction;
    /* we update the bar to 0, n times
     * except when the value is the original value
     */
    for (int x = 0; x < n; x++) {
        if (x != original) {
            *direction = x;
            cubes[v][*i][*j][*k]= 0;
        }
    }
    // to reset the variable to its original value
    *direction = original;
}

/* set the row, column, possible values and grid for a cell to zero
 * except for the given cell
 */
void updateCell(int i, int j, int k) {
    setBar(&i, &j, &k, &i);
    setBar(&i, &j, &k, &j);
    setBar(&i, &j, &k, &k);
    setGrid(i, j, k);
}
