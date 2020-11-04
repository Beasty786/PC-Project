#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define SIZE 6
// #define SIZE 6

#define SIZE 30
#define MASTER 0
int grid[SIZE*SIZE];
int followingGen[SIZE*SIZE];



void setGrid(int grid[SIZE*SIZE], int followingGen[SIZE*SIZE]);
int countNeighbors(int x , int y , int grid[SIZE][SIZE]);


int main(int argc, char * argv[]) {

    int numtasks, rank, source, dest, chunksize, offset, tag1, tag2;
    

    MPI_Status status;
    MPI_Init( & argc, & argv);
    MPI_Comm_size(MPI_COMM_WORLD, & numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, & rank);

    tag1 = 1; // tag for chunk size
    tag2 = 2; // tag for offset
    source = MASTER;

    if (rank == 0) {
        chunksize = (SIZE*SIZE) / numtasks;
        offset = (SIZE*SIZE) % numtasks;
        printf("chunksize is %d\n offset is %d\n", chunksize, offset);
        for (int i = 0; i < numtasks - 1; i++) {
            dest = i + 1;

            offset += chunksize;
            MPI_Send( & chunksize, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
            MPI_Send( & offset, 1, MPI_INT, dest, tag2, MPI_COMM_WORLD);
        }
        chunksize = (SIZE % numtasks) + chunksize;
        offset = 0;
    } else {
        MPI_Recv( & chunksize, 1, MPI_INT, MASTER, tag1, MPI_COMM_WORLD, & status);
        MPI_Recv( & offset, 1, MPI_INT, MASTER, tag2, MPI_COMM_WORLD, & status);

    }


    printf("The offset for task %d is %d and chunksize is %d\n", rank, offset, chunksize);
    MPI_Finalize();




    return 0;
}

void setGrid(int grid[SIZE*SIZE], int followingGen[SIZE*SIZE]) {
    for (int i = 0; i < SIZE*SIZE; i++) {
            grid[i] = rand() % 2;
            followingGen[i] = 0;
        
    }
}



int countNeighbors(int x, int y , int grid[SIZE][SIZE]) {
    int sum = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int r = (SIZE + y + i) % SIZE;
            int c = (SIZE + x + j) % SIZE;
            if (grid[r][c] == 1) {
                sum = sum + 1;
            }
        }
    }
    if (grid[y][x] == 1) {
        sum = sum - 1;
    }
    return sum;
}