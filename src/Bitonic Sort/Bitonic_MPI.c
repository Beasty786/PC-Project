#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

bool isBitonic(int *arr , int size , int index);
void swap(int* arr , int i , int j);
void printArr(int *arr , int size);
int changes(int * arr , int size);
void arrUpdate(int *arr , int size , int index);
int min(int a , int b);
void indices(int *array ,int mid , int length , int inv);
void reset(int *arr , int size);
void init(int * arr0 , int *arr , int size);

int main(int argc, char const *argv[])
{
    
    int size ;
    scanf("%d", &size);
    int arr0[size/2];   // indices to perform magic sort
    int arr[size];      // Bitonic sequence array
    int mid = size/2;   // where to split
    int inv = mid*2;    // inverse magic number
    int indexLength = size / 2;
    init(arr0 , arr , size);
    clock_t t;
    t = clock();
    while(mid > 0){ 
        #pragma omp parallel num_threads(8)
        {
            #pragma omp for
            for(int i = 0; i < size/2 ; i++){
                int ind = arr0[i];
                if(arr[ind+mid] <= arr[ind]){
                    swap(arr , ind + mid , ind);
                }
            }   
            
            if(omp_get_thread_num() == 0){  
                mid = mid/2;
                inv = mid *2;
               // printArr(arr , size);
                if(mid > 0){
                    indices(arr0 , mid , indexLength , inv);
                } 
            }
            
        }    
    }
    t = clock()-t;
    printArr(arr,size);
    printf("time taken is %f\n\n " , ((double)t)/CLOCKS_PER_SEC);

    return 0;
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