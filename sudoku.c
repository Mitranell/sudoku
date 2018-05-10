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

#define SOLUTION_FOUND 1
#define SOLUTION_NOT_FOUND 0

MPI_Request request;
MPI_Status status;

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

int solved = 0;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"

int root, maybe_root;
int i_am_root = 0;


int main(int argc, char *argv[]) {
    // init MPI
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);

    readSudoku();

    if (timer(&solve)) {
        cubeToSudoku();
        i_am_root = 1;
    }
    maybe_root = (i_am_root ? thread_rank : 0);
    MPI_Allreduce(&maybe_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Bcast(&solved, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (thread_rank == root){
        outputSudoku();
        printf("Thread: %d \n Duration: %f\n\n", thread_rank, duration);
    }

    MPI_Finalize();

    return 0;
}
