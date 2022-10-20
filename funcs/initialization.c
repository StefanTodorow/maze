#include "funcs.h"

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

int countExitPoints(Grid grid) {
    int exitsCount = 0;
    for (int i = 1; i < grid.cols-1; i++) // top border
    {
        if (grid.grid[0][i] == 0 ||
        (grid.grid[0][i] >= 101 && grid.grid[0][i] <= 120))
        {
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // left border
    {
        if (grid.grid[i][0] == 0 ||
        (grid.grid[i][0] >= 101 && grid.grid[i][0] <= 120))
        {
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // right border
    {
        if (grid.grid[i][grid.cols-1] == 0 ||
        (grid.grid[i][grid.cols-1] >= 101 && grid.grid[i][grid.cols-1] <= 120))
        {
            exitsCount++;
        }
    }
    for (int i = 1; i < grid.cols-1; i++) // bottom border
    {
        if (grid.grid[grid.rows-1][i] == 0 ||
        (grid.grid[grid.rows-1][i] >= 101 && grid.grid[grid.rows-1][i] <= 120))
        {
            exitsCount++;
        }
    }
    return exitsCount;
}

void initExitPoints(Grid grid, ExitPoint* exitPoints) {
    // check every border for possible exit point
    int iterator = 0;
    for (int i = 1; i < grid.cols-1; i++) // top border
    {
        if (grid.grid[0][i] == 0 ||
        (grid.grid[0][i] >= 101 && grid.grid[0][i] <= 120))
        {
            exitPoints[iterator].x = 0;
            exitPoints[iterator].y = i;
            iterator++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // left border
    {
        if (grid.grid[i][0] == 0 ||
        (grid.grid[i][0] >= 101 && grid.grid[i][0] <= 120))
        {
            exitPoints[iterator].x = i;
            exitPoints[iterator].y = 0;
            iterator++;
        }
    }
    for (int i = 1; i < grid.rows-1; i++) // right border
    {
        if (grid.grid[i][grid.cols-1] == 0 ||
        (grid.grid[i][grid.cols-1] >= 101 && grid.grid[i][grid.cols-1] <= 120))
        {
            exitPoints[iterator].x = i;
            exitPoints[iterator].y = grid.cols-1;
            iterator++;
        }
    }
    for (int i = 1; i < grid.cols-1; i++) // bottom border
    {
        if (grid.grid[grid.rows-1][i] == 0 ||
        (grid.grid[grid.rows-1][i] >= 101 && grid.grid[grid.rows-1][i] <= 120))
        {
            exitPoints[iterator].x = grid.rows-1;
            exitPoints[iterator].y = i;
            iterator++;
        }
    }

    printf("%d exit/s\n", iterator);
    for (int i = 0; i < iterator; i++)
    {
        printf("(%d,%d)\n", exitPoints[i].x, exitPoints[i].y);
    }
    
    if (!iterator)
    {
        // if there is no exit/s
        error(NoExitError);
    }
}