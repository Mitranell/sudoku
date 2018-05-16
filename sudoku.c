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
struct freeCells { int length, cell *arr};
struct dualCell { int i1,j1,i2,j2; };
clock_t start;
double duration;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"


int logx (int x, int base) {
    return floor(log(x) / log(base));
}

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

    // The grid has more cells then. Adding further levels.
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
        int depth = n * logx(nprocs, n); // IS IT TIMES N???????????????????

        for (int i = thread_rank; i < n; i += nprocs) {
                readSudoku();

                cell *emptyCells = findVariableAmountOfEmptyCells(depth * n);

                int noOfEmptyCells = 
                for ()

                for (int j = 0; j < depth; j++) {

                    updateCell(backtrackCell.i, backtrackCell.j, i);

              
                    
                    if (solve()) {
                        solved = 1;
                        possible_root = thread_rank;
                        break;
                    }
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
