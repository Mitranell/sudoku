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
int not_broadcasting = 1;
int task_receiver = 0;

int const GIVE_TASK = 100;
int const GIVE_TASK_ANSWER = 101;
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

MPI_Request request;
MPI_Request request2;
MPI_Status status;

#include "update.c"
#include "output.c"
#include "check.c"
#include "solve.c"

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
        MPI_Irecv(&solvedByThread, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
    }

    /* set possible_root to the current rank if the sudoku is solved
     */
    int possible_root = 0;
    int level = ceil(logf((float)nprocs)/logf((float)n));
    // TODO test if different levels improve the solution time
    // level += 1;
    for (int i = thread_rank; i < (int)pow((double)n, (double)level); i += nprocs) {
        /* check if broadcast is ongoing, if no start asynch broadcast */
        if (i!= 0)
            MPI_Test(&request2, &not_broadcasting, &status);
        if (not_broadcasting)
            MPI_Ibcast(&solvedByThread, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD, &request2);

        if (solvedByThread == -1){
            readSudoku();
            
            int m = i;
            for (int j = level; j > 0; j--) {
                struct cell backtrackCell = findEmpty();

                int k = (int)(m / pow(n, j-1));
                m -= k*pow(n,j - 1);
                updateCell(backtrackCell.i, backtrackCell.j, k);
            }

            if (solve()) {
                solved = 1;
                possible_root = thread_rank;
                /* if rank 0 problems with sending and receiving on same thread
                 * therefore set solveByThread manually when on rank 0 */
                if (thread_rank != 0)
                    MPI_Isend(&possible_root, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,&request);
                else
                    solvedByThread = 0;

                break;
            }
        } else break;
    }

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
