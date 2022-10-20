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
    case NoExitError:
        printf("There is no exit point/s.");
        break;
    default:
        break;
    }
    exit(err);
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
    if (pP.x < 0 || pP.x >= grid.rows)
    {
        error(PlayerPosError);
    }
    else if (pP.y < 0 || pP.y >= grid.cols)
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