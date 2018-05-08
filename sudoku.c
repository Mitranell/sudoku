#include <stdio.h>
#include <time.h>
// #include <iostream>
// #include <fstream>
#include <limits.h>
// #include <sys/time.h>
// #include <omp.h>
// #include <stdexcept>
#include <stdlib.h>

int l;
int n;
int total_sum;
int grid[4];
int** sudoku;
int*** cube;
struct cell { int i,j; };
clock_t start;
double duration;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"


int main(int argc, char *argv[]) {
    readSudoku();
    outputSudoku();
    timer(&solve);
    //solve();
    cubeToSudoku();
    outputSudoku();
    printf("Duration: %f\n\n", duration);
    return 0;
}
