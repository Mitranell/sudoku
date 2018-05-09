/* outputs the sudoku
 */
void outputSudoku() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(sudoku[i][j] > n) {
                cout << sudoku[i][j] << endl;
                throw invalid_argument("To large number");
            }
            cout << sudoku[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/* outputs the sudoku
 */
void outputSudoku(int** sudoku) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(sudoku[i][j] > n) {
                cout << sudoku[i][j] << endl;
                throw invalid_argument("To large number");
            }
            cout << sudoku[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/* outputs the row [i][*][k] of the 3d cube
 */
void outputRow(int i, int k) {
    for (int j = 0; j < n; j++) {
        cout << cube[i][j][k]<< " ";
    }
    cout << endl;
}

/* outputs the column [*][j][k] of the 3d cube
 */
void outputColumn(int j, int k) {
    for (int i = 0; i < n; i++) {
        cout << cube[i][j][k] << endl;
    }
    cout << endl;
}

/* outputs the depth [i][j][*] of the 3d cube
 */
void outputDepth(int i, int j) {
    for (int k = 0; k < n; k++) {
        cout << cube[i][j][k] << " ";
    }
    cout << endl;
}

/* outputs the rows and columns [*][*][k] of the 3d cube
 */
void outputSliceCube(int k, int n) {
    for (int x = 0; x < n; x++) {
        outputRow(x, k);
    }
}
