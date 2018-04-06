#include <iostream>
#include <vector>


using namespace std;
int*** cube;
int n;

#include "output.cpp"

int main(){
    n = 9;
    cube = new int**[n];
    for (int i = 0; i < n; i++) {
        cube[i] = new int*[n];
        for (int j = 0; j < n; j++) {
            cube[i][j] = new int[n];
            for (int k = 0; k < n; k++) {
                cube[i][j][k] = 1;
            }
        }
    }


    outputRow(0,0);

    //vector< vector< vector<int>>> cube;
    vector<int> test;
    test[0] = 1;
    cout << endl<< test[0]; 
    //cout << "hallo\n";
}
