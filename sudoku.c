#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <mpi.h>

// mpi
int once = 1;
int root;
int thread_rank;
int solved = 0;

// serial
int l;
int n;
int total_sum;
int grid[4];
int **sudoku;
int ***cube;
struct cell { int i,j; };
clock_t start;
double duration;

int solvedByOtherThread = 0;

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

    // all threads read the sudoku and only on thread outputs it
    readSudoku();
    if (thread_rank == 0) {
        outputSudoku();
    }

    //listener for solutions
    int solvedByOtherThread = 0;
    // set possible_root to the current rank if the sudoku is solved
    int possible_root = 0;
    if (timer(&solve)) {
        solved = 1;
        possible_root = thread_rank;
    }
    MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&solved, &solvedByOtherThread, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    // take the maximal rank of possible roots
    //MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    // only the choosen root outputs the sudoku
    if (thread_rank == root){
        if (solved == 1) {
            cubeToSudoku();
            outputSudoku();
            printf("Thread: %d \nDuration: %f\n\n", thread_rank, duration);
        } else {
            printf("No solution\n");
        }
    }

    MPI_Finalize();

    return 0;
}
