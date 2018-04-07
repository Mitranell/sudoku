using namespace std;

#include <vector>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>

typedef vector<int> v1d;
typedef vector<v1d> v2d;
typedef vector<v2d> v3d;
typedef vector<v3d> v4d;

int l;
int n;
int total_sum;
int* grid;
v4d cubes;
int v = 0;
struct flag_struct {
    int i,j,k;
};
vector<flag_struct> v_flag;
// TODO: comment
flag_struct intial = {-1,-1,-1};
clock_t start;
double duration;

#include "update.cpp"
#include "output.cpp"
#include "check.cpp"
#include "bruteforce.cpp"
#include "main.cpp"


int main(int argc, char *argv[]) {
    // TODO: comment
    v_flag.push_back(intial);

    outputSudoku(readSudoku());
    timer(&solve);
    outputSudoku(cubeToSudoku());

    return 0;
}
