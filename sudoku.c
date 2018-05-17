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
int solvedByThread = -1;
int flag = 1;
/*struct Solution {
    int solvedByOtherThread;
    int thread;
} solution={0,0};*/
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
char *file;

#include "update.c"
#include "output.c"
#include "check.c"
#include "main.c"

MPI_Request request;
MPI_Request request2;
MPI_Status status;

int main(int argc, char *argv[]) {
    start = clock();
    // init MPI
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);

    // read arguments command line
    if( argc == 2 ) {
        file = argv[1];
        if (!file) {
            if (thread_rank == 0) {
                printf("File does not exist or cannot be opened.");
            }
            MPI_Finalize();
            return 0;
        }
    } else if( argc > 2 ) {
        if (thread_rank == 0) {
            printf("Too many arguments supplied.\n");
        }
        MPI_Finalize();
        return 0;
    } else {
        if (thread_rank == 0) {
            printf("Please enter the name of the sudoku file.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // all threads read the sudoku and only on thread outputs it
    readSudoku();
    if (thread_rank == 0) {
        printf("Initial sudoku:\n");
        outputSudoku();
    }
    /* receiver on thread 0 */
    if (thread_rank == 0){
        MPI_Irecv(&solvedByThread, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    }
    /* try several possible values for the first empty cell
     * example thread 2 and 5 processors: 2, 7, 12, 17, ...
     * set possible_root to the current rank if the sudoku is solved
     */
    int possible_root = 0;
    for (int i = thread_rank; i < n; i += nprocs) {
        MPI_Test(request2, &flag, &status));
        if (flag)
            MPI_Ibcast(&solvedByThread, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request2); 
        if (solvedByThread == -1){
            readSudoku();
            struct cell backtrackCell = findEmptyCell();
            updateCell(backtrackCell.i, backtrackCell.j, i);

            if (solve()) {
                solved = 1;
                possible_root = thread_rank;
                printf("solved: before all reduce, %d\n", thread_rank);
                if (thread_rank != 0)
                    MPI_Isend(&possible_root, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,&request);
                else
                    solvedByThread = 0;
                //MPI_Allreduce(&possible_root, &solvedByThread, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
                printf("solved: after all reduce, %d\n", thread_rank);
                break;
            }
        }
    }
    /*if (!solved){
        MPI_Allreduce(&possible_root, &solvedByThread, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    }*/
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&solvedByThread, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD); 


    // only the choosen root outputs the sudoku

    if (solvedByThread == thread_rank) {
        duration = (clock() - start) / (double) CLOCKS_PER_SEC;
        printf("Solution:\nThread: %d \nDuration: %f\n\n", thread_rank, duration);
        cubeToSudoku();
        outputSudoku();
    } 

   
    MPI_Finalize();

    return 0;
}
