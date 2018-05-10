#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <mpi.h>

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
int found_solution = 0;


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

    if (timer(&solve)) {
        cubeToSudoku();
        found_solution = thread_rank;
    }

    MPI_Allreduce(&found_solution, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    //MPI_Bcast(&solved, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (thread_rank == root){
        outputSudoku();
        printf("Thread: %d \nDuration: %f\n\n", thread_rank, duration);
    }

    MPI_Finalize();

    return 0;
}
