/* outputs the sudoku
 */
void outputSudoku() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ",sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* outputs the sudoku
 */
void outputSudokuWithParam(int sudoku[][]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ",sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/* outputs the row [i][*][k] of the 3d cube
 */
void outputRow(int i, int k) {
    for (int j = 0; j < n; j++) {
        printf("%d ",cube[i][j][k]);
    }
    printf("\n");
}

/* outputs the column [*][j][k] of the 3d cube
 */
void outputColumn(int j, int k) {
    for (int i = 0; i < n; i++) {
        printf("%d ",cube[i][j][k]);
    }
    printf("\n");
}

/* outputs the depth [i][j][*] of the 3d cube
 */
void outputDepth(int i, int j) {
    for (int k = 0; k < n; k++) {
        printf("%d ",cube[i][j][k]);
    }
    printf("\n");
}

/* outputs the rows and columns [*][*][k] of the 3d cube
 */
void outputSliceCube(int k, int n) {
    for (int x = 0; x < n; x++) {
        outputRow(x, k);
    }
}
