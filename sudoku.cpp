using namespace std;

#include <vector>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>

int l;
int n;
int total_sum;
int* grid = new int[4];
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
    
    outputSudoku(readSudoku());
    timer(&solve);
    outputSudoku(cubeToSudoku());

    return 0;
}
