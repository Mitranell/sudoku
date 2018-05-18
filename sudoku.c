#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
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
// struct FreeCells { int length, cell *arr};
// struct dualCell { int i1,j1,i2,j2; };
clock_t start;
double duration;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"


// int logx (int x, int base) {
//     return floor(log(x) / log(base));
// }
//
// int howOftenFits(int small, int big) {
//     int count = 0;
//     int comp = small;
//
//     while (comp <= big) {
//         comp += small;
//         count++;
//     }
//
//     return comp;
//
// }
//
// int getSmallestPower(int base, int comp) {
//     for (int i = 0; i < INT_MAX; i++) {
//         if (comp < (int)pow(base, i)) {
//             return comp;
//         }
//     }
// }


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

    // // The grid has more cells then. Adding further levels.
    // if (nprocs > n) {
    //
    //     int depth = n * logx(nprocs, n); // IS IT TIMES N???????????????????
    //
    //     for (int i = thread_rank; i < n; i += nprocs) {
    //         int leftOver = -1;
    //         for (int j = 0; j < depth; j++) {
    //             readSudoku();
    //
    //             int smallestPow;
    //             if (j == 0) {
    //                 smallestPow = getSmallestPower(n, i);
    //             } else {
    //                 smallestPow = getSmallestPower(n, leftOver);
    //             }
    //
    //             int decreasedVal = pow(n, smallestPow - 1);
    //
    //             int res = howOftenFits(decreasedVal, i);
    //
    //             int leftOver = i - res;
    //
    //             struct cell backtrackCell = findEmpty();
    //             updateCell(backtrackCell.i, backtrackCell.j, res);
    //
    //             if (solve()) {
    //                 solved = 1;
    //                 possible_root = thread_rank;
    //                 break;
    //             }
    //         }
    //     }
    // }

    for (int i = thread_rank; i < n; i += nprocs) {
        readSudoku();
        int level = ceil(logf((float)nprocs)/logf((float)n));

        for (int j = 0; j < level; j++) {
            struct cell backtrackCell = findEmpty();

            int k = ceil(i / n^(level - 1));
            printf("Thread %d with node %d filles %d at level %d\n", thread_rank, i, k, level);
            i -= (k - 1)*n^(level - 1);
            updateCell(backtrackCell.i, backtrackCell.j, k);
        }

        if (solve()) {
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
