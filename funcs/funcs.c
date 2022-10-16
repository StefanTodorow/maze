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
    case GridSizeError:
        printf("Grid size not valid.");
        break;
    case GridFieldError:
        printf("Grid field/s not valid.");
        break;
    case PlayerPosError:
        printf("Player position not valid.");
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

FILE* openOutput(char* fName) {
    FILE* fp = fopen(fName, "w");
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

void validateGrid(Grid grid) {
    if (grid.rows >= 50 || grid.rows < 1)
    {
        error(GridSizeError);
    }
    else if (grid.cols >= 50 || grid.cols < 1)
    {
        error(GridSizeError);
    }

    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            if (grid.grid[i][j] < -1 || grid.grid[i][j] > 120)
            {
                error(GridFieldError);
            }
            else if (grid.grid[i][j] > 20 &&
                    grid.grid[i][j] <= 100)
            {
                error(GridFieldError);
            }
        }
    }
}

void validatePlayerPos(PlayerPos pP, Grid grid) {
    if (pP.x < 1 || pP.x > grid.rows)
    {
        error(PlayerPosError);
    }
    else if (pP.y < 1 || pP.y > grid.cols)
    {
        error(PlayerPosError);
    }
    else if (grid.grid[pP.x][pP.y] == -1)
    {
        error(PlayerPosError);
    }
    else if (grid.grid[pP.x][pP.y] >= 101 &&
            grid.grid[pP.x][pP.y] <= 120)
    {
        error(PlayerPosError);
    }
}

int findAllExitPoints(Grid grid, ExitPoint* exitPoints) {
    int exitsCount = 0;
    // make loops in func
    for (int i = 1; i < grid.cols-1; i++) // top border
    {
        if (grid.grid[0][i] == 0)
        {
            exitPoints[exitsCount].x = 0;
            exitPoints[exitsCount].y = i;
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // left border
    {
        if (grid.grid[i][0] == 0)
        {
            exitPoints[exitsCount].x = i;
            exitPoints[exitsCount].y = 0;
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // right border
    {
        if (grid.grid[i][grid.cols-1] == 0)
        {
            exitPoints[exitsCount].x = i;
            exitPoints[exitsCount].y = grid.cols-1;
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.cols-1; i++) // bottom border
    {
        if (grid.grid[grid.rows-1][i] == 0)
        {
            exitPoints[exitsCount].x = grid.rows-1;
            exitPoints[exitsCount].y = i;
            exitsCount++;
        }
    }

    // if(grid.grid[0][0] == 0) { //top left corner
    //     exitPoints[exitsCount].x = 0;
    //     exitPoints[exitsCount].y = 0;
    //     exitsCount++;
    // }
    // if(grid.grid[0][grid.cols-1] == 0) { //top right corner
    //     exitPoints[exitsCount].x = 0;
    //     exitPoints[exitsCount].y = grid.cols-1;
    //     exitsCount++;
    // }
    // if(grid.grid[grid.rows-1][0] == 0) { //bottom left corner
    //     exitPoints[exitsCount].x = grid.rows-1;
    //     exitPoints[exitsCount].y = 0;
    //     exitsCount++;
    // }
    // if(grid.grid[grid.rows-1][grid.cols-1] == 0) { //top left corner
    //     exitPoints[exitsCount].x = grid.rows-1;
    //     exitPoints[exitsCount].y = grid.cols-1;
    //     exitsCount++;
    // }

    printf("%d exit/s\n", exitsCount);
    for (int i = 0; i < exitsCount; i++)
    {
        printf("(%d,%d)\n", exitPoints[i].x, exitPoints[i].y);
    }
    
    if (!exitsCount)
    {
        puts("No exit point/s.");
        exit(1);
    }

    return exitsCount;
}

int isSafe(Grid grid, int** seen, int x, int y) {
    return (x >= 0 && x < grid.rows && y >= 0 && y < grid.cols) &&
            grid.grid[x][y] == 0 && !seen[x][y];
}

void tryPaths(Grid grid, int** seen, int i, int j, ExitPoint* exits, int exitsCount, int* minDist, int currDist) {
    for (int e = 0; e < exitsCount; e++)
    {
        if (i == exits[e].x && j == exits[e].y)
        {
            *minDist = currDist < *minDist ? currDist : *minDist;
            return;
        }
    }

    seen[i][j] = 1;

    // go to bottom field
    if (isSafe(grid, seen, i+1, j)) 
    {
        tryPaths(grid, seen, i+1, j, exits, exitsCount, minDist, currDist+1);
    }

    // go to right field
    if (isSafe(grid, seen, i, j+1)) 
    {
        tryPaths(grid, seen, i, j+1, exits, exitsCount, minDist, currDist+1);
    }
    // go to top field
    if (isSafe(grid, seen, i-1, j)) 
    {
        tryPaths(grid, seen, i-1, j, exits, exitsCount, minDist, currDist+1);
    }
    // go to left field
    if (isSafe(grid, seen, i, j-1)) 
    {
        tryPaths(grid, seen, i, j-1, exits, exitsCount, minDist, currDist+1);
    }
    
    seen[i][j] = 0;
}

int shortestExit(Grid grid, PlayerPos plPos, ExitPoint* exits, int exitsCount) {
    int minDist = __INT_MAX__;
    int** seen = (int**)calloc(grid.rows, sizeof(int));
    if(seen == NULL) {
        error(MemoryError);
    }
    for (int i = 0; i < grid.rows; ++i)
    {
        seen[i] = (int*)calloc(grid.cols, sizeof(int));
        if(seen[i] == NULL) {
            error(MemoryError);
        }
    }
    tryPaths(grid, seen, plPos.x, plPos.y, exits, exitsCount, &minDist, 0);

    if (minDist != __INT_MAX__)
    {
        return minDist;
    }
    return -1;
}