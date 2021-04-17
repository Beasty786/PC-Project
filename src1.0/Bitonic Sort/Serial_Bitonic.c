#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

bool isBitonic(int *arr , int size , int index);
void swap(int* arr , int i , int j);
void printArr(int *arr , int size);
int changes(int * arr , int size);
void arrUpdate(int *arr , int size , int index);
void updateIndices(int *array ,int mid , int length , int inv);
double BitonicSort(int *arr , int *indices , int mid , int inv , int size);
void init(int * arr0 , int *arr , int size);

int main(int argc, char const *argv[])
{   printf("You are now running the serial code for bitonic sort");
    printf("\nPLEASE ENTER THE SIZE OF THE ARRAY AND PRESS ENTER \nTHEN ENTER THE VALUES OF THE ARRAY YOU WISH TO SORT IN A BITONIC SEQUENCE\n\n");

    int size ;
    scanf("%d" , &size);
    int arr[size];
    int arr0[size/2];
    init(arr0 , arr , size);
   
    bool isbit = isBitonic(arr , size , 0);
    if (isbit)
    {
        int mid = size/2;
        int inv = mid*2;
        double time = BitonicSort(arr , arr0 ,mid,inv , size );
        printArr(arr, size);
        printf("the time taken is %f\n",time);
    }else{
        // The lines below tries to rotate the numbers in case the sequence entered is a rotational
        // Bitonic sequence, it then checks again if the rotated is bitonic, if not it tells what's the issue
        int index = changes(arr , size);
        arrUpdate(arr , size , index);
        index = changes(arr , size);
        arrUpdate(arr , size , index);
        if(isBitonic(arr, size , 0)){

            int mid = size/2;
            int inv = mid*2;
            double time = BitonicSort(arr , arr0 ,mid,inv , size );
            printArr(arr, size);
            printf("the time taken is %f\n",time);
        }
        else{
            printf("Please reRun the program and enter a bitonic sequence!");
        }
    }
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

double BitonicSort(int *arr , int *indices , int mid , int inv , int size){
    clock_t t;
    t = clock();
    int arrSize = size/2;
    
    while(mid > 0){
        for(int i = 0 ; i < arrSize ; i++){
            int ind = indices[i];
            if(arr[ind+mid] <= arr[ind]){
                swap(arr , ind + mid , ind);
            }
        }

        mid /= 2;
        inv = mid*2;
        if(mid > 0){
            updateIndices(indices , mid , arrSize , inv);
        }
    }
    t = clock() - t;
    return ((double)t / CLOCKS_PER_SEC);
}

void updateIndices(int *array ,int mid , int length , int inv){
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
            index = i;
            count++;
        }
      }
      else
      {
        if(arr[i+1] > arr[i]){
            isIncreasing = true;
            index = i;
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