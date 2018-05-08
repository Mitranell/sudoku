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
#include <mpi.h>

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

int getMPIRank() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    return rank;
}


int main(int argc, char *argv[]) {
    readSudoku();
    outputSudoku();

    // init MPI
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // get PID
    int me;
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    printf("Hi from node %d of %d\n", me, nprocs);
    
    timer(&solve);
    //solve();

    MPI_Finalize();

    cubeToSudoku();
    outputSudoku();
    cout << "Duration: "<< duration << endl << endl;
    return 0;
}
