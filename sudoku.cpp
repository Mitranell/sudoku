/**
    Parallel and Distributed Computing
    Purpose: Create a parallel version of sudoku.

    @author 
    @version 1.1 04/08/18
*/

using namespace std;

#include <vector>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>
#include <stdexcept>

int l;
int n;
int total_sum;
int* grid = new int[4];
int** sudoku;
int*** cube;
struct cell { int i,j; };
cell backtrack;
clock_t start;
double duration;

#include "update.cpp"
#include "output.cpp"
#include "check.cpp"
#include "main.cpp"



int main(int argc, char *argv[]) {
    readSudoku();
    outputSudoku();
    timer(&solve);
    //solve();
    cubeToSudoku();
    outputSudoku();
    cout << "Duration: "<< duration << endl << endl;
    return 0;
}
