#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

bool isBitonic(int *arr , int size , int index);
void swap(int* arr , int i , int j);
void printArr(int *arr , int size);
int changes(int * arr , int size);
void arrUpdate(int *arr , int size , int index);
void bitonicSort(int *arr , int size);

int main(int argc, char const *argv[])
{
    //int arr[8] = {1 , 2 ,  4 , 5 , 9 , 8 , 7 , 6};
   // int arr[] = {8 , 9 , 2 , 1 , 0 , 4 ,9};
    int arr[] = {3,5,8,9,10,12,14,20,95,90,60,40,35,23,18,0};
    int size = sizeof(arr)/ sizeof(int);

    printArr(arr, size);
    int index = changes(arr , size);
    printf("\n");
    printf("%d",index);
    printf("\n");

    arrUpdate(arr , size , index);
    printf("\n");
    printArr(arr , size);
    printf("\n");

    index = changes(arr , size);
    printf("\n");
    printf("%d",index);
    printf("\n");

    arrUpdate(arr , size , index);
    printf("\n");
    printArr(arr , size);
    printf("\n");

    bool isbit = isBitonic(arr , size , 0);
    if (isbit)
    {
        bitonicSort(arr , size);
    }
    printArr(arr, size);
    return 0;
}

void bitonicSort(int *arr, int size) 
{ 
    int i, k = size; 

    while (k > 0) { 
        for (i = 0; i + k < size; i++) 
            if (arr[i] > arr[i + k]) 
                swap(arr,i, i + k); 
  
        k = k / 2; 
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