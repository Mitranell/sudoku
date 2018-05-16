#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <mpi.h>

/* the number of values need to be guessed by backtracking before we listen
 * to the left neighbor to see if it needs search space
 */
#define CHECK 10;

// mpi
int once = 1;
int root;
int thread_rank;
int solved = 0;
int count = 0;
MPI_Status status;
int iprobe_flag;

// serial
int l;
int n;
int total_sum;
int grid[4];
int **sudoku;
int ***cube;
struct cell { int i,j; };
struct dualCell { int i1,j1,i2,j2; };
clock_t start;
double duration;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"


int main(int argc, char *argv[]) {
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

    // set possible_root to the current rank if the sudoku is solved
    int possible_root = 0;

    // The grid has more cells then. Adding 2nd level.
    if (nprocs > n) {
        for (int i = thread_rank; i < n; i += nprocs) {
            for (int j = thread_rank; j < n; j += nprocs) {
                readSudoku();
                struct dualCell backtrackCell = findEmptyCells();
                updateCell(backtrackCell.i1, backtrackCell.j1, i);
                updateCell(backtrackCell.i2, backtrackCell.j2, j);

                if (solve()) {
                    solved = 1;
                    possible_root = thread_rank;
                    break;
                }
            }
        }
    } else {
        for (int i = thread_rank; i < n; i += nprocs) {
            readSudoku();
            struct cell backtrackCell = findEmptyCell();
            updateCell(backtrackCell.i, backtrackCell.j, i);

            if (solve()) {
                solved = 1;
                possible_root = thread_rank;
                break;
            }
        }

        int buffer;
        for (int i = 0; i < n; i++) {
            if (thread_rank != i) {
                MPI_Send(&buffer, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    }

    // take the maximal rank of possible roots
    MPI_Allreduce(&possible_root, &root, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

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
