/* for each row and column: check all possible cell values for uniqueness
 */
void bruteforceCell(int i, int j) {
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

    if (sum > 1){
        //TODO: check if this p was already was in a previous bruteforce iteration 
        //update cell mit v = v+1;
        cubes.push_back(cubes[v]);
        v++;
        updateCell(i,j,p);
        flag_struct tmp = { i,j,p };
        v_flag.push_back(tmp);
    }
}

/* just started, so not correct*/
void bruteforce(){

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

            // if the sum equals 1, then it was the only one
            if (sum == 1) {
                updateCell(i,j,p);
            }

        }
    }

}

