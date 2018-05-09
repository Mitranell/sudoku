#include <stdio.h>
#include <time.h>
// #include <iostream>
// #include <fstream>
#include <limits.h>
// #include <sys/time.h>
// #include <omp.h>
// #include <stdexcept>
#include <stdlib.h>
#include <mpi.h>

int once = 1;
int thread_rank;
int l;
int n;
int total_sum;
int grid[4];
int **sudoku;
int ***cube;
struct cell { int i,j; };
clock_t start;
double duration;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"


int main(int argc, char *argv[]) {
    // init MPI
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
    printf("Hi from node %d of %d\n", thread_rank, nprocs);

    if (thread_rank == 0) {
        readSudoku();
        outputSudoku();
    }

    timer(&solve);
    //solve();

    if (thread_rank == 0) {
        cubeToSudoku();
        outputSudoku();
        printf("Duration: %f\n\n", duration);
    }

    MPI_Finalize();

    return 0;
}
