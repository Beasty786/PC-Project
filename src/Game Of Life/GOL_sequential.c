#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define SIZE 6
// #define SIZE 6
#define SIZE 75

int p[SIZE][SIZE];

void setGrid();
void printGrid();
void updateGrid();
int countNeighbors( int x , int y);

int main(int argc, char const *argv[])
{
    setGrid();
    printGrid();

    
        for(int i = 0 ; i < 1; i++){
            updateGrid();
        }
        
        printf("\n\n");
        int j = 100000000;
        while (j > 0)
        {
            j--;
        }
        //system("clear");
        printGrid();
    
    
    
    return 0;
}

void setGrid(){
    for(int i = 0 ; i < SIZE ; i++){
        for(int j = 0 ; j < SIZE ; j++){
            int x = rand()%2;
            p[i][j] = x;
        }
    }
}

void printGrid(){
     for(int i = 0 ; i < SIZE ; i++){
        for(int j = 0 ; j < SIZE ; j++){

            if(p[i][j] == 1)
            printf("* ");
            else
            {
                printf("  ");
            }
            
        }
        printf("\n");
    }
}

void updateGrid(){
    int nextGen[SIZE][SIZE];

    for(int i = 0 ; i < SIZE ; i++){
        for(int j = 0 ; j < SIZE ; j++){
            int state = p[i][j];
            int neighbors = countNeighbors(j , i);
           // printf("state {%d, %d} has %d neighbours \n", i , j , neighbors);
            if(state == 0 && neighbors == 3){
                nextGen[i][j] = 1;
            }
            else if(state == 1 && (neighbors < 2 || neighbors >3)){
                nextGen[i][j] = 0;
            }
            else
            {
                nextGen[i][j] = p[i][j];
            }
            
        }
        
    }

      for(int i = 0; i < SIZE ; i++){
            for(int j = 0 ; j < SIZE ; j++){

                    p[i][j] = nextGen[i][j];
            
        }
      }
}

int countNeighbors( int x , int y){   
    int sum = 0;
     for(int i = -1 ; i < 2 ; i++){
        //  if(y == 0 && i == -1) continue;
        //  if(y == SIZE -1 && i == 1) continue;
        for(int j = -1 ; j < 2 ; j++){
            // if(x == 0 && j == -1) continue;
            // if(x == SIZE -1 && j == 1) continue;
            int r = (SIZE + y+i)%SIZE;
            int c = (SIZE + x+j)%SIZE;
            if(p[r][c] == 1){
                sum = sum + 1;
            }
        }
    }
    if(p[y][x] == 1){
                sum = sum - 1;
    }
    return sum;
}