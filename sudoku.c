#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/* the number of values need to be guessed by backtracking before we listen
 * to the left neighbor to see if it needs search space
 */
#define CHECK 10;

// mpi
int once = 1;
int root;
int thread_rank;
int solved = 0;
MPI_Status status;
MPI_Request request;
int iprobe_flag;
int buffer[3];
int data[3];
int tag = 0;

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
        printf("\nThread %d did %d solve\n", thread_rank, solved);

        /* TODO: thread_rank != 0 is temporarely
         * If the root asks for search space while other threads have found
         * solutions there will be a deadlock
         */
        if (!solved && thread_rank != 0) {
            tag += thread_rank;
            for (int i = 0; i < nprocs; i++) {
                if (thread_rank != i) {
                    // asking for search space
                    printf("%d asks %d for search space with tag %d\n", thread_rank, i, tag);
                    MPI_Isend(&buffer, 3, MPI_INT, i, tag, MPI_COMM_WORLD, &request);
                }
            }

            printf("%d is waiting for a response\n", thread_rank);
            // a thread has responded to the request for search space
            MPI_Recv(&data, 3, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
            printf("%d received \nI: %d J: %d K: %d\nfrom %d\n\n", thread_rank, data[0], data[1], data[2], status.MPI_SOURCE);
            // let the thread know that we have accepted the request
            buffer[0] = data[0];
            buffer[1] = data[1];
            buffer[2] = data[2];
            printf("%d is accepting %d\n\n", thread_rank, status.MPI_SOURCE);
            MPI_Send(&buffer, 3, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
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
