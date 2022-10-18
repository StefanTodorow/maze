#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

void error(Errors err) {
    //function to print errors
    switch (err)
    {
    case FileError:
        printf("Error with file.");
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
    // set player's positon
    PlayerPos plP;
    fscanf(input, "%d", &plP.x);
    fscanf(input, "%d", &plP.y);

    return plP;
}

Grid initGrid(FILE* input) {
    // set grid rows, cols, elements
    Grid grid;
    fscanf(input, "%d", &grid.rows);
    fscanf(input, "%d", &grid.cols);

    // allocate memory for grid's rows
    grid.grid = (int**)malloc(grid.rows * sizeof(int));
    if (grid.grid == NULL)
    {
        error(MemoryError);
    }
    
    for (int i = 0; i < grid.rows; i++)
    {   
        // set columns to grid rows
        grid.grid[i] = (int*)malloc(grid.cols * sizeof(int));
        if (grid.grid[i] == NULL)
        {
            error(MemoryError);
        }

        // get the elements
        for (int j = 0; j < grid.cols; j++)
        {
            fscanf(input, "%d", &grid.grid[i][j]);
        }
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
    // free every column first
    for (int i = 0; i < grid.rows; i++)
    {
        free(grid.grid[i]);
    }
    // free rows
    free(grid.grid);
}

void validateGrid(Grid grid) {
    // validate size of grid
    if (grid.rows >= 50 || grid.rows < 1)
    {
        error(GridSizeError);
    }
    else if (grid.cols >= 50 || grid.cols < 1)
    {
        error(GridSizeError);
    }

    // validate each element
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
    // if out of bounds
    if (pP.x < 1 || pP.x > grid.rows)
    {
        error(PlayerPosError);
    }
    else if (pP.y < 1 || pP.y > grid.cols)
    {
        error(PlayerPosError);
    }
    else if (grid.grid[pP.x][pP.y] == -1) // if player is on wall
    {
        error(PlayerPosError);
    }
    else if (grid.grid[pP.x][pP.y] >= 101 && // if player is on door
            grid.grid[pP.x][pP.y] <= 120)
    {
        error(PlayerPosError);
    }
}

int findAllExitPoints(Grid grid, ExitPoint* exitPoints) {
    int exitsCount = 0;
    for (int i = 1; i < grid.cols-1; i++) // top border
    {
        if (grid.grid[0][i] == 0 ||
        (grid.grid[0][i] >= 101 && grid.grid[0][i] <= 120))
        {
            exitPoints[exitsCount].x = 0;
            exitPoints[exitsCount].y = i;
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // left border
    {
        if (grid.grid[i][0] == 0 ||
        (grid.grid[i][0] >= 101 && grid.grid[i][0] <= 120))
        {
            exitPoints[exitsCount].x = i;
            exitPoints[exitsCount].y = 0;
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // right border
    {
        if (grid.grid[i][grid.cols-1] == 0 ||
        (grid.grid[i][grid.cols-1] >= 101 && grid.grid[i][grid.cols-1] <= 120))
        {
            exitPoints[exitsCount].x = i;
            exitPoints[exitsCount].y = grid.cols-1;
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.cols-1; i++) // bottom border
    {
        if (grid.grid[grid.rows-1][i] == 0 ||
        (grid.grid[grid.rows-1][i] >= 101 && grid.grid[grid.rows-1][i] <= 120))
        {
            exitPoints[exitsCount].x = grid.rows-1;
            exitPoints[exitsCount].y = i;
            exitsCount++;
        }
    }

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

int isSafe(Grid grid, int** seen, int* doorTimers, int x, int y) {
    // check if next field is ok to step on
    int nextField = grid.grid[x][y];
    if (x >= 0 && x < grid.rows && y >= 0 && y < grid.cols && !seen[x][y])
    {
        if (nextField == 0)
        {
            return 1;
        }
        else if (nextField >= 1 && nextField <= 20)
        {
            return 1;
        }
        else if(nextField >= 101 && nextField <= 120 && 
        doorTimers[nextField-101] < 20 && doorTimers[nextField-101] != 0)
        {
            return 1;
        }
    }
    return 0;
}

void increasedoorTimers(int* doorTimers) {
    for (int i = 0; i < 20; i++)
    {
        if (doorTimers[i] != 0)
        {
            doorTimers[i]++;
            //printf("\ndoor timer %d: %d\n", i, doorTimers[i]);
        }
    }
}

void tryPaths(Grid grid, int** seen, int* doorTimers, int i, int j, ExitPoint* exits, int exitsCount, int* minDist, int currDist) {
    // loop through every possible exit point
    for (int e = 0; e < exitsCount; e++)
    {   
        // if player is on an exit point
        if (i == exits[e].x && j == exits[e].y)
        {
            currDist++;
            // if the current distance is shorter than the minimum distance
            // set the minimum distance to current distance
            *minDist = currDist < *minDist ? currDist : *minDist;
            return;
        }
    }

    increasedoorTimers(doorTimers);

    int currentField = grid.grid[i][j];
    if (currentField >= 1 && currentField <= 20)
    {
        doorTimers[currentField-1] = 1;
    }

    // set current position of player to be seen
    seen[i][j] = 1;

    // go to bottom field
    if (isSafe(grid, seen, doorTimers, i+1, j)) 
    {
        tryPaths(grid, seen, doorTimers, i+1, j, exits, exitsCount, minDist, currDist+1);
    }

    // go to right field
    if (isSafe(grid, seen, doorTimers, i, j+1)) 
    {
        tryPaths(grid, seen, doorTimers, i, j+1, exits, exitsCount, minDist, currDist+1);
    }
    // go to top field
    if (isSafe(grid, seen, doorTimers, i-1, j)) 
    {
        tryPaths(grid, seen, doorTimers, i-1, j, exits, exitsCount, minDist, currDist+1);
    }
    // go to left field
    if (isSafe(grid, seen, doorTimers, i, j-1)) 
    {
        tryPaths(grid, seen, doorTimers, i, j-1, exits, exitsCount, minDist, currDist+1);
    }

    // go to bottom field
    if (isSafe(grid, seen, doorTimers, i+1, j)) 
    {
        tryPaths(grid, seen, doorTimers, i+1, j, exits, exitsCount, minDist, currDist+1);
    }

    // go to right field
    if (isSafe(grid, seen, doorTimers, i, j+1)) 
    {
        tryPaths(grid, seen, doorTimers, i, j+1, exits, exitsCount, minDist, currDist+1);
    }
    // go to top field
    if (isSafe(grid, seen, doorTimers, i-1, j)) 
    {
        tryPaths(grid, seen, doorTimers, i-1, j, exits, exitsCount, minDist, currDist+1);
    }
    // go to left field
    if (isSafe(grid, seen, doorTimers, i, j-1)) 
    {
        tryPaths(grid, seen, doorTimers, i, j-1, exits, exitsCount, minDist, currDist+1);
    }
    
    // backtracking
    seen[i][j] = 0;
}

int shortestExit(Grid grid, PlayerPos plPos, ExitPoint* exits, int exitsCount) {
    int minDist = __INT_MAX__;
    int doorTimers[20] = {};
    
    // make a matrix to keep track of which fields are seen
    int** seen = (int**)calloc(grid.rows, sizeof(int));
    if(seen == NULL) {
        error(MemoryError);
    }

    for (int i = 0; i < grid.rows; ++i)
    {
        // set columns to the seen rows
        seen[i] = (int*)calloc(grid.cols, sizeof(int));
        if(seen[i] == NULL) {
            error(MemoryError);
        }
    }

    tryPaths(grid, seen, doorTimers, plPos.x, plPos.y, exits, exitsCount, &minDist, 0);

    // free seen matrix
    for (int i = 0; i < grid.rows; i++)
    {
        free(seen[i]);
    }
    free(seen);

    if (minDist != __INT_MAX__)
    {
        return minDist;
    }
    // if minimum Distance is unchanged it means there is no exit
    return 0;
}