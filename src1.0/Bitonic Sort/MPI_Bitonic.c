#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
 * MPI applications: mpicc hello_world.c -o test
 * to run: 
 *  mpiexec -n 16 ./test
 *  mpirun -np 16 ./test
*/

bool isBitonic(int *arr , int size , int index);
void swap(int* arr , int i , int j);
void printArr(int *arr , int size);
int changes(int * arr , int size);
void arrUpdate(int *arr , int size , int index);
int min(int a , int b);
void indices(int *array ,int mid , int length , int inv);
void reset(int *arr , int size);
void init(int * arr0 , int *arr , int size);
int numLevels(int mid);
bool isSorted(int *arr, int size);

int main(int argc, char *argv[])
{
    
    int size;
    //scanf("%d", &size);
    // Bitonic sequence array
    
    int mid;                                 // where to split
    int inv;                                   // inverse magic number
    int indexLength;
    int levels = numLevels(size);
    
    int numtasks , rank , tag1 = 1 , tag2 = 2;
    clock_t t;
    MPI_Status stat, stat2;
    
    MPI_Init(&argc , &argv);
    MPI_Comm_size(MPI_COMM_WORLD , &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD , &rank);

    if(rank == 0){
        printf("\nHOPEFULLY YOU DID SPECIFY THE NUMBER OF PROCESSORS USING THE COMMAD BELOW \n*** mpirun -np 16 ...*** ");
        printf("\nPLEASE ENTER THE SIZE OF THE ARRAY AND PRESS ENTER \nTHEN ENTER THE VALUES OF THE ARRAY YOU WISH TO SORT IN A BITONIC SEQUENCE\n\n");
        scanf("%d", &size);
    }
    MPI_Bcast(&size , 1 , MPI_INT , 0 , MPI_COMM_WORLD);
    int arr[size];
    int arr0[size/2];

    if(rank == 0){   
        init(arr0 , arr , size);
    }
    
    MPI_Bcast(&arr,size,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&arr0 , size/2 , MPI_INT , 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    mid = size/2;                                   // where to split
    inv = mid*2;                                    // inverse magic number
    indexLength = size / 2;
    levels = numLevels(size);

    int indicesPerProcessor = indexLength / numtasks ;
    t = clock();
    for(int k = 0 ; k < levels ; k++){
       MPI_Barrier(MPI_COMM_WORLD);
        for(int i = rank*indicesPerProcessor; i < rank*indicesPerProcessor + indicesPerProcessor ; i++){
                int ind = arr0[i];
                if(arr[arr0[i]+mid] < arr[arr0[i]]){
                    swap(arr , arr0[i] + mid , arr0[i]);
                }
        }

        if(rank != 0){
            MPI_Send(&arr , size ,MPI_INT , 0 , tag1 , MPI_COMM_WORLD);    
        }
        if(rank == 0){
            int temparr[size];
            for(int j = 1 ; j < numtasks ; j++){
                
                MPI_Recv(&temparr , size, MPI_INT , j ,tag1 , MPI_COMM_WORLD, &stat);
                for(int i = j*indicesPerProcessor; i < j*indicesPerProcessor + indicesPerProcessor ; i++){
                    arr[arr0[i]] = temparr[arr0[i]];
                    arr[arr0[i]+mid] = temparr[arr0[i] + mid];
                }
            }   
        }

        mid = mid/2;
        inv = mid*2;
        if(mid > 0 && rank == 0){
            indices(arr0 , mid , indexLength , inv);
        } 
        MPI_Bcast(&arr,size,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&arr0 , size/2 , MPI_INT , 0, MPI_COMM_WORLD);

    }
    t = clock() - t;
    
    if(isSorted(arr , size) && rank == 0){
        printf("\nThe array is sorted and time taken is %f \n\n", (double)t / CLOCKS_PER_SEC);
    }
 
    MPI_Finalize();
    if(rank == 0)
        printArr(arr,size);
}

bool isSorted(int *arr , int size){
    for(int i = 0; i < size-1 ; i++){
        if(arr[i] > arr[i+1]) return false;
    }
    return true;
}
int numLevels(int mid){
    int m = mid;
    int count = 0;
   for(int i = mid ; mid > 1 ; mid/=2){
       count ++;
   }
    return count; 
}

void init(int * arr0 , int *arr , int size){
    
    for(int i = 0 ; i < size ; i++ ){
        if(i < size/2){
            arr0[i] = i;
        }
        scanf("%d" , &arr[i]);
    }
}


void indices(int *array ,int mid , int length , int inv){
    int index = 0;
    int j = 0;
    for(int i = mid ; i < length ; i++){
       if(j%mid == 0 ){
           index = index + inv;
           j=0;
       }
       array[i]  = index + j;
       j++;

    }
}

int changes(int *arr , int size){
    int count = 0;
    int index = -1;
    bool isIncreasing = false;
    if(arr[0] < arr[1]){
        isIncreasing = true;
    }

        int i;
    for (i = 0; i < size-1; i++)
    {
      if (isIncreasing)
      {
        if(arr[i+1] < arr[i]){
            isIncreasing = false;
            index = i+1;
            count++;
        }
      }
      else
      {
        if(arr[i+1] > arr[i]){
            isIncreasing = true;
            index = i+1;
            count ++;
        }
      }     
    }

    return count < 3 ? index : -1;
}

void arrUpdate(int* arr , int size , int index){
    int newArr[size];
    for (size_t i = 0; i < size; i++)
    {
        newArr[i] = arr[index%size];
        index ++;
    }

    for (size_t i = 0; i < size; i++)
    {
        arr[i] = newArr[i];
    }
    
}

bool isBitonic(int* arr , int size , int index){
    bool bitonic = true;
    
    bool isIncreasing = false;
    if(arr[0] < arr[1]){
        isIncreasing = true;
    }

    int i;
    for (i = 0; i < size-1; i++)
    {
      if (isIncreasing)
      {
          if(arr[i+1] < arr[i]){
              isIncreasing = false;
              i = i + 1;
              break;
          }
      }
      else
      {
           if(arr[i+1] > arr[i]){
              isIncreasing = true;
              i = i + 1;
              break;
            }
      }     
    }

    for (size_t j = i; j < size-1; j++)
    {
        if(isIncreasing){
            if (arr[j] > arr[j+1])
            {
                bitonic = false;
                break;
            }
            
        }
        else
        {
            if (arr[j] < arr[j+1])
            {
                bitonic = false;
                break;
            }
        }
        
    }
    

    return bitonic;
}

void swap(int *arr , int i , int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void printArr(int *arr , int size){
    for (size_t i = 0; i < size; i++)
    {
        printf("%d " , arr[i]);
    }
    printf("\n");
}