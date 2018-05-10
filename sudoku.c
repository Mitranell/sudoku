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

    // set possible_root to the current rank if the sudoku is solved
    int possible_root = 0;
    for (int i = thread_rank; i < nprocs; i += nprocs) {
        readSudoku();
        struct cell backtrackCell = findEmptyCell();
        updateCell(backtrackCell.i, backtrackCell.j, i);
        printf("Thread: %d\nValue: %d\n\n", thread_rank, i);
        outputSudoku();

        if (timer(&solve)) {
            solved = 1;
            possible_root = thread_rank;
            break;
        }
    }

    // take the maximal rank of possible roots
    MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

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
