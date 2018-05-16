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
FILE *sudoku_file;

int solvedByOtherThread = 0;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"

int main(int argc, char *argv[]) {
    if( argc == 2 ) {
        sudoku_file = fopen(argv[1], "r");
        printf(sudoku_file);
        if (!sudoku_file) {
            printf("File does not exist or cannot be opened.");
            return 0;
        }
    } else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
        return 0;
    } else {
        printf("Please enter the name of the sudoku file.\n");
        return 0;
    }

    start = clock();
    // init MPI
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);

    // all threads read the sudoku and only on thread outputs it
    readSudoku();
    if (thread_rank == 0) {
        printf("Initial sudoku:\n");
        outputSudoku();
    }

    /* try several possible values for the first empty cell
     * example thread 2 and 5 processors: 2, 7, 12, 17, ...
     * set possible_root to the current rank if the sudoku is solved
     */
    int solvedByOtherThread = 0;
    int possible_root = 0;
    for (int i = thread_rank; i < n; i += nprocs) {
        readSudoku();
        struct cell backtrackCell = findEmptyCell();
        updateCell(backtrackCell.i, backtrackCell.j, i);

        if (solve()) {
            solved = 1;
            possible_root = thread_rank;

            MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
            MPI_Allreduce(&solved, &solvedByOtherThread, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

            break;
        }
        MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
        MPI_Allreduce(&solved, &solvedByOtherThread, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    }

    //MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    //MPI_Allreduce(&solved, &solvedByOtherThread, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    // take the maximal rank of possible roots
   // MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    // only the choosen root outputs the sudoku
    if (thread_rank == root){
        if (solved == 1) {
            duration = (clock() - start) / (double) CLOCKS_PER_SEC;
            printf("Solution:\nThread: %d \nDuration: %f\n\n", thread_rank, duration);
            cubeToSudoku();
            outputSudoku();
        } else {
            printf("No solution\n");
        }
    }

    MPI_Finalize();

    return 0;
}
