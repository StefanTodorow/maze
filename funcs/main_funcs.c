#include "funcs.h"

int tE_isSafe(Grid grid, int** seen, Door* doors, Key* keys, int x, int y) {
    // check if next field is ok to step on

    // if position is out of bounds
    if (x >= grid.rows || y >= grid.cols || x < 0 || y < 0)
    {
        return 0;
    }
    
    int nextField = grid.grid[x][y];

    // if it is a key save it
    if (nextField >= 1 && nextField <= 20)
    {
        keys[nextField-1].isReachable = 1;
        keys[nextField-1].x = x;
        keys[nextField-1].y = y;
    }
    // if it is a door save it
    else if (nextField >= 101 && nextField <= 120)
    {
        doors[nextField-101].isReachable = 1;
        doors[nextField-101].x = x;
        doors[nextField-101].y = y;
    }

    // if it is inbounds and haven't stepped on it
    if (x >= 0 && x < grid.rows && y >= 0 && y < grid.cols && !seen[x][y])
    {   
        // if it is a wall
        if (nextField == -1)
        {
            return 0;
        }
        // if it is a door
        else if (nextField >= 101 && nextField <= 120)
        {
            return 0;
        }
        
        return 1;
    }
    return 0;
}

void tryExit(Grid grid, int** seen, int i, int j, ExitPoint* exits, int exitsCount, Door* doors, Key* keys, int* minDist, int currDist) {
    // tryExit looks for an exit point to escape the maze, also sees and saves keys and doors on the way
    
    int currentField = grid.grid[i][j];

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

    // set current position of player to be stepped on
    seen[i][j] = 1;

    // go to bottom field
    if (tE_isSafe(grid, seen, doors, keys, i+1, j)) 
    {
        tryExit(grid, seen, i+1, j, exits, exitsCount, doors, keys, minDist, currDist+1);
    }

    // go to right field
    if (tE_isSafe(grid, seen, doors, keys, i, j+1)) 
    {
        tryExit(grid, seen, i, j+1, exits, exitsCount, doors, keys, minDist, currDist+1);
    }
    // go to top field
    if (tE_isSafe(grid, seen, doors, keys, i-1, j)) 
    {
        tryExit(grid, seen, i-1, j, exits, exitsCount, doors, keys, minDist, currDist+1);
    }
    // go to left field
    if (tE_isSafe(grid, seen, doors, keys, i, j-1)) 
    {
        tryExit(grid, seen, i, j-1, exits, exitsCount, doors, keys, minDist, currDist+1);
    }
    
    // backtracking
    seen[i][j] = 0;
}

int sP_isSafe(Grid grid, int** seen, int x, int y) {
    // check if next field is ok to step on
    if (x >= 0 && x < grid.rows && y >= 0 && y < grid.cols && !seen[x][y])
    {
        if (grid.grid[x][y] == -1)
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

void shortestPath(Grid grid, int** seen, int i, int j, int endX, int endY, int* shortestP, int currDist) {
    // shortestPath finds the shortest path from point A to point B

    int currentField = grid.grid[i][j];

    // if the end is found
    if (i == endX && j == endY)
    {   
        // save the length of the path
        *shortestP = currDist < *shortestP ? currDist : *shortestP;
        return;
    }
    

    // set current position of player to be seen
    seen[i][j] = 1;

    // go to bottom field
    if (sP_isSafe(grid, seen, i+1, j))
    {
        shortestPath(grid, seen, i+1, j, endX, endY, shortestP, currDist+1);
    }

    // go to right field
    if (sP_isSafe(grid, seen, i, j+1)) 
    {
        shortestPath(grid, seen, i, j+1, endX, endY, shortestP, currDist+1);
    }
    // go to top field
    if (sP_isSafe(grid, seen, i-1, j)) 
    {
        shortestPath(grid, seen, i-1, j, endX, endY, shortestP, currDist+1);
    }
    // go to left field
    if (sP_isSafe(grid, seen, i, j-1)) 
    {
        shortestPath(grid, seen, i, j-1, endX, endY, shortestP, currDist+1);
    }
    
    // backtracking
    seen[i][j] = 0;
}

int shortestExit(Grid grid, PlayerPos plPos) {
    int minDist = __INT_MAX__; // shortest path to exit
    int pathLen = 0; // from starting point to keys to doors...to exit
    int currentX = plPos.x;
    int currentY = plPos.y;
    int startingField = grid.grid[currentX][currentY];
    
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

    int exitsCount = countExitPoints(grid);
    // array containing each exit point on border of grid
    ExitPoint* exits = (ExitPoint*)malloc(exitsCount * sizeof(ExitPoint));
    if (exits == NULL)
    {
        error(MemoryError);
    }
    initExitPoints(grid, exits);

    Door doors[20] = {};
    Key keys[20] = {};

    // if starting field is a key save it
    if (startingField >= 1 && startingField <= 20)
    {
        keys[startingField-1].isReachable = 1;
        keys[startingField-1].x = currentX;
        keys[startingField-1].y = currentY;
    }

    // trials to exit
    int trials = 20;
    while (trials--)
    {
        // check if player can exit now
        tryExit(grid, seen, currentX, currentY, exits, exitsCount, doors, keys, &minDist, pathLen);
        // if player exited minDist will change
        if (minDist != __INT_MAX__)
        {
            // free seen matrix
            for (int i = 0; i < grid.rows; i++)
            {
                free(seen[i]);
            }
            free(seen);

            // free grid
            for (int i = 0; i < grid.rows; i++)
            {
                free(grid.grid[i]);
            }
            // free rows
            free(grid.grid);
            
            return minDist;
        }

        // print saved doors and keys
        for (int i = 0; i < 20; i++)
        {
            if (doors[i].isReachable && !doors[i].isUsed)
            {
                printf("door %d: %d, %d\n", i, doors[i].x, doors[i].y);
            }
        }
        for (int i = 0; i < 20; i++)
        {
            if (keys[i].isReachable && !keys[i].isUsed)
            {
                printf("key %d: %d, %d\n", i, keys[i].x, keys[i].y);
            }
        }

        // iterate for every key/door to go through them
        for (int i = 0; i < 20; i++)
        {
            int currentPathLen = __INT_MAX__;

            // if the door and key are reachable now and are not used
            if (doors[i].isReachable && keys[i].isReachable && !doors[i].isUsed && !keys[i].isUsed)
            {
                // if we are on it right now
                if (currentX == keys[i].x && currentY == keys[i].y)
                {
                    // use the key
                    keys[i].isUsed = 1;
                }
                // if not find the shortest path to the corresponding key and save it
                else {
                    shortestPath(grid, seen, currentX, currentY, keys[i].x, keys[i].y, &currentPathLen, 0);
                    keys[i].isUsed = 1;
                    currentX = keys[i].x;
                    currentY = keys[i].y;
                    pathLen += currentPathLen; // update path's length
                    currentPathLen = __INT_MAX__; // reset current path's length
                    printf("currentPathLen %d\n", pathLen);
                }
                // now find the shortest path to the corresponding door from the key and save it
                shortestPath(grid, seen, currentX, currentY, doors[i].x, doors[i].y, &currentPathLen, 0);
                doors[i].isUsed = 1;
                currentX = doors[i].x;
                currentY = doors[i].y;

                pathLen += currentPathLen; // update path's length
                printf("currentPathLen %d\n", pathLen);
                break;
            }
        }
    }

    // if minimum Distance is unchanged it means there is no exit so return 0
    return 0;
}