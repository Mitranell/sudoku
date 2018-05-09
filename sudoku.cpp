using namespace std;

#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>
#include <stdexcept>
#include <mpi.h>

#define SOLUTION_FOUND 1
#define SOLUTION_NOT_FOUND 0

MPI_Request request;
MPI_Status status;

int once = 1;
int thread_rank;
int l;
int n;
int total_sum;
int* grid = new int[4];
int** sudoku;
int*** cube;
struct cell { int i,j; };
cell backtrack;
clock_t start;
double duration;

bool solved = false;

#include "update.cpp"
#include "output.cpp"
#include "check.cpp"
#include "main.cpp"

// int getMPIId() {
//     int thread_rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
//
//     return thread_rank;
// }


int main(int argc, char *argv[]) {
    readSudoku();
    outputSudoku();

    // init MPI
    int nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    // get rank
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_rank);
    printf("Hi from node %d of %d\n", thread_rank, nprocs);

    // thread 0 listens with irecv for solutions
    if (thread_rank == 0 ){
        int[][] sudokuOtherThreads = new int[n][n];
        MPI_Irecv(sudokuOtherThreads, n*n, MPI_INT, MPI_ANY_SOURCE, SOLUTION_FOUND, MPI_COMM_WORLD, &request);
        sudoku = sudokuOtherThreads;
        outputSudoku();
    }

    timer(&solve);
    //solve();


    cubeToSudoku();

    // if thread != 0 and found solution, send to thread 0
    if (thread_rank != 0 && solved){
        MPI_send(sudoku, n*n, MPI_INT, 0, SOLUTION_FOUND, MPI_COMM_WORLD);
    } else {
        outputSudoku(sudoku);
    }

    outputSudoku();
    cout << "Duration: "<< duration << endl << endl;   

    MPI_Abort(MPI_COMM_WORLD);


    MPI_Finalize();

    return 0;
}
