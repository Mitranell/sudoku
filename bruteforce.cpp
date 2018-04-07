// just checks if the k is a valid k
bool rightDepth(int i, int j, int k){
    if (cubes.size() == v + 1 ) {
        return true;
    } else {
        //checks if this k was selected on the last step
        return v_flag[v+1].i != i ||
               v_flag[v+1].j != j ||
               (v_flag[v+1].k != k && v_flag[v+1].i == i && v_flag[v+1].j == j);
    }

}

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
            if (rightDepth(i,j,k)) {
                cout << "setting cell k + 1" << k + 1 << endl;
                cout << "v_flag[v].k: " << v_flag[v].k << endl;
                if (cubes.size() == v +1 ) {
                    cubes.push_back(cubes[v]);
                } else {
                    cubes[v+1] = cubes[v];
                    cin.ignore();
                }

                v++;
                //crash at updatecell
                //cout << cubes[v][i][j][k];
                cout << "v: " << v << endl;
                updateCell(i,j,k);
                flag_struct tmp = { i,j,k };
                v_flag.push_back(tmp);
                outputDepth(i,j);
                break;
            }
        }
    }
}

/* finds the first cell which has multiple possibilities and bruteforces on it
 */
int bruteforce(){
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
                return 0;
            }
        }
    }
}

void revertBruteforceStep(){
    cout << "reverting bruteforce step " << v << endl;
    v_flag.resize(v_flag.size()-1);
    //cubes[v].erase(cubes[v].begin());
    v--;
}
