#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

void error(Errors err) {
    switch (err)
    {
    case FileError:
        printf("Cannot open INPUT.txt.");
        break;
    case MemoryError:
        printf("Cannot allocate memory.");
        break;
    default:
        break;
    }
    exit(err);
}

FILE* openInput(char* fName) {
    FILE* fp = fopen(fName, "r");
    if(fp == NULL) {
        error(FileError);
    }
    return fp;
}

PlayerPos initPlayer(FILE* input) {
    PlayerPos plP;
    fscanf(input, "%d", &plP.x);
    fscanf(input, "%d", &plP.y);

    return plP;
}

Grid initGrid(FILE* input) {
    Grid grid;
    fscanf(input, "%d", &grid.rows);
    fscanf(input, "%d", &grid.cols);

    grid.grid = (int**)malloc(grid.rows * sizeof(int));
    if (grid.grid == NULL)
    {
        error(MemoryError);
    }
    
    for (int i = 0; i < grid.rows; i++)
    {
        int* col = (int*)malloc(grid.cols * sizeof(int));
        if (col == NULL)
        {
            error(MemoryError);
        }

        for (int j = 0; j < grid.cols; j++)
        {
            fscanf(input, "%d", &col[j]);
        }
        grid.grid[i] = col;
    }

    return grid;
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

void freeGrid(Grid grid) {
    for (int i = 0; i < grid.rows; i++)
    {
        free(grid.grid[i]);
    }
    free(grid.grid);
}