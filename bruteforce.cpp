// just checks if the k is a valid k
bool rightDepth(int i, int j, int k){
    if (v_flag.size() == v + 1 ){
        return true;
    } else {
        //checks if this k was selected in the last steps
        bool result = true;
        for (int m = v+1; m < v_flag.size() ; m++ ){
            if ( v_flag[m].i != i || v_flag[m].j != j ) return result;
            result = result && (v_flag[m].k != k && v_flag[m].i == i && v_flag[m].j == j);
        }
        return result;
    }
    
}

void bruteforceCell(int i, int j) {
    cout << "bruteforcing cell " << i << " " << j << endl;
    outputDepth(i,j);
    int sum = 0;S
        // loop over all the cell its possible values
    for (int k = 0; k < n; k++) {
        sum += cubes[v][i][j][k];
        // save the right depth
        if (cubes[v][i][j][k]) {
            if (rightDepth(i,j,k)){
                cout << "setting cell k " << k << endl;
                cout << "v_flag[v].k: " << v_flag[v].k << endl;
                //v_flag.resize(v_flag.size()-1);
                if (cubes.size() == v +1 ){
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
                if (v_flag.size() == v +1 ){
                    v_flag.push_back(tmp);
                } else {
                     v_flag[v+1] = tmp;
                    
                }
               
                outputDepth(i,j);
                break;
            }
        }
    }
}



/* just started, so not correct*/
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
    //v_flag.resize(v_flag.size()-1);
    //cubes[v].erase(cubes[v].begin());
    v--;
    
}

