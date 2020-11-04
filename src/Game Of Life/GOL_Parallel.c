#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 75
#define MASTER 0
int grid[SIZE][SIZE];
int followingGen[SIZE][SIZE];



void setGrid(int grid[SIZE][SIZE], int followingGen[SIZE][SIZE]);
void printGrid(int g[SIZE][SIZE]);
int countNeighbors(int x, int y);
void MPI_Parallel(int argc, char * argv[]);
void MPIupdatefollowingGen(int chunkSize, int offset, int followingGen[SIZE][SIZE]);
void MPIupdateGrid(int chunkSize, int offset, int followingGen[SIZE][SIZE]);
void MPIprintGrid(int chunksize, int offset);
void MPIprintArray(int arr[SIZE]);
void MPIcopyArr(int top[SIZE] ,int bottom[SIZE], int offset,int chunksize);
void printMypart(int g[SIZE][SIZE] , int offset , int chunksize) ;

int main(int argc, char * argv[]) {
    setGrid(grid, followingGen);

    int numtasks, rank, source, dest, chunksize, offset, tag1, tag2;
    int top[SIZE], bottom[SIZE] , t[SIZE] , b[SIZE];
    

    MPI_Status status1 ,status2;
    MPI_Init( & argc, & argv);
    MPI_Comm_size(MPI_COMM_WORLD, & numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, & rank);

    tag1 = 1; // tag for chunk size
    tag2 = 2; // tag for offset
    source = MASTER;
    chunksize = (SIZE) / numtasks;
    offset = (SIZE) % numtasks;
    if(rank == MASTER){

        chunksize = (SIZE % numtasks) + chunksize;
        offset = 0;
    }
    for (int i = 0; i < numtasks - 1; i++) {
            if(rank== i + 1){
                offset += chunksize*rank; 
            };     
    }
  
  
    for(int i = 0 ; i < numtasks ; i++){
          MPI_Barrier(MPI_COMM_WORLD);
        if(rank == i){
           
            for(int j = 0 ; j < SIZE ; j++){
               
                top[j] = grid[offset][j];
                bottom[j]=grid[offset+chunksize-1][j];
               
            }
            MPI_Send(&top , SIZE, MPI_INT , (numtasks+(i-1))%numtasks , tag1 , MPI_COMM_WORLD);
            MPI_Send(&bottom , SIZE, MPI_INT , (numtasks+(i+1))%numtasks , tag2 , MPI_COMM_WORLD);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Recv(&b , SIZE , MPI_INT , (rank+1)%numtasks ,tag1, MPI_COMM_WORLD,&status1);
    MPI_Recv(&t, SIZE , MPI_INT , (numtasks+(rank -1))%numtasks , tag2 , MPI_COMM_WORLD , &status2);

    MPIcopyArr(t , b , offset , chunksize);
    MPIupdatefollowingGen(chunksize,offset,followingGen);
    MPIupdateGrid(chunksize , offset , followingGen);

     for(int i = 0 ; i < numtasks ; i++){
        MPI_Barrier(MPI_COMM_WORLD);
        if(rank == i){
            printMypart(grid , offset , chunksize);
        }
    }
    

    MPI_Finalize();

    return 0;
}

void MPIprintArray(int arr[SIZE]){
    printf("[ ");
    for(int i = 0 ; i < SIZE ; i++){
        if(i == 0){
            printf("%d ",arr[i]);
        }
        else
        {
            printf(" , %d" , arr[i]);
        }
    }
    printf(" ]\n");
}

void MPIcopyArr(int top[SIZE], int bottom[SIZE] , int offset ,int chunksize){
    for(int i = 0 ; i < SIZE ; i ++){
        grid[(SIZE+offset-1)%SIZE][i] = top[i];
        grid[(offset+chunksize+1)%SIZE][i] = bottom[i];
    }
}

void setGrid(int grid[SIZE][SIZE], int followingGen[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int x = rand() % 2;
            grid[i][j] = x;
            followingGen[i][j] = 1;
        }
    }
}

void printGrid(int g[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (g[i][j] == 1)
                printf("* ");
            else {
                printf("  ");
            }

        }
        printf("\n");
    }
}

void printMypart(int g[SIZE][SIZE] , int offset , int chunksize) {
    for (int i = offset; i < offset+chunksize; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (g[i][j] == 1)
                printf("* ");
            else {
                printf("  ");
            }

        }
        printf("\n");
    }
}

void MPIprintGrid(int chunksize, int offset) {
    for (int i = offset; i < offset + chunksize; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (grid[i][j] == 1)
                printf("* ");
            else {
                printf("  ");
            }

        }
        printf("\n");
    }
}

void MPIupdateGrid(int chunkSize, int offset, int followingGen[SIZE][SIZE]) {

    for (int i = offset; i < offset + chunkSize; i++) {
        for (int j = 0; j < SIZE; j++) {

            grid[i][j] = followingGen[i][j];

        }
    }
}

void MPIupdatefollowingGen(int chunkSize, int offset, int followingGen[SIZE][SIZE]) {

    for (int i = offset; i < chunkSize + offset; i++) {
        for (int j = 0; j < SIZE; j++) {
            int state = grid[i][j];
            int neighbors = countNeighbors(j, i);
            if (state == 0 && neighbors == 3) {
                followingGen[i][j] = 1;
            } else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                followingGen[i][j] = 0;
            } else {
                followingGen[i][j] = grid[i][j];
            }

        }

    }


}


int countNeighbors(int x, int y) {
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