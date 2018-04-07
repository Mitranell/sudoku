/* for each row and column: check all possible cell values for uniqueness
 */
void bruteforceCell(int i, int j) {
    cout << "bruteforcing cell " << i << " " << j << endl;
    outputDepth(i,j);
    int sum = 0;
    int p;
        // loop over all the cell its possible values
    for (int k = 0; k < n; k++) {
        sum += cubes[v][i][j][k];
        // save the right depth
        if (cubes[v][i][j][k]) {
            if (v_flag[v].i != i || v_flag[v].j != j ||  (v_flag[v].k !=k && v_flag[v].i == i && v_flag[v].j == j)){
                cout << "setting cell k " << k << endl;
                cubes.push_back(cubes[v]);
                v++;
                //crash at updatecell
                updateCell(i,j,p);
                flag_struct tmp = { i,j,p };
                v_flag.push_back(tmp);
                break;
            }
        }
    }

    if (sum > 1){
        //TODO: check if this p was already was in a previous bruteforce iteration 
        //update cell mit v = v+1;

    }
}

/* just started, so not correct*/
void bruteforce(){
    cout << "bruteforce started" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            int p;

            // loop over all the cell its possible values
            for (int k = 0; k < n; k++) {
                sum += cubes[v][i][j][k];
            }

            // if the sum equals 1, then it was the only one
            if (sum > 1) {
                bruteforceCell(i,j);
            }

        }
    }

}

void revertBruteforceStep(){
    cout << "reverting bruteforce step " << v << endl;
    v_flag.resize(v_flag.size()-1);
    cubes[v].erase(cubes[v].begin());
    v--;
    
}

