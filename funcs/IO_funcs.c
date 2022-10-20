#include "funcs.h"

FILE* openInput(char* fName) {
    FILE* fp = fopen(fName, "r");
    if(fp == NULL) {
        error(FileError);
    }
    return fp;
}

FILE* openOutput(char* fName) {
    FILE* fp = fopen(fName, "w");
    return fp;
}

void printGrid(Grid grid) {
    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            printf("%d ", grid.grid[i][j]);
        }
        puts("");
    }
}