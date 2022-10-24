#include "funcs.h"

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

void shortestPath(Grid grid, int** seen, int i, int j, int key, int* shortestP, int currDist, int* newX, int* newY) {
    // shortestPath finds the shortest path from point A to point B

    int currentField = grid.grid[i][j];

    // if the end is found
    if (currentField == key)
    {   
        *newX = i;
        *newY = j;
        // save the length of the path
        *shortestP = currDist < *shortestP ? currDist : *shortestP;
        return;
    }
    

    // set current position of player to be seen
    seen[i][j] = 1;

    // go to bottom field
    if (sP_isSafe(grid, seen, i+1, j))
    {
        shortestPath(grid, seen, i+1, j, key, shortestP, currDist+1, newX, newY);
    }

    // go to right field
    if (sP_isSafe(grid, seen, i, j+1)) 
    {
        shortestPath(grid, seen, i, j+1, key, shortestP, currDist+1, newX, newY);
    }
    // go to top field
    if (sP_isSafe(grid, seen, i-1, j)) 
    {
        shortestPath(grid, seen, i-1, j, key, shortestP, currDist+1, newX, newY);
    }
    // go to left field
    if (sP_isSafe(grid, seen, i, j-1)) 
    {
        shortestPath(grid, seen, i, j-1, key, shortestP, currDist+1, newX, newY);
    }
    
    // backtracking
    seen[i][j] = 0;
}

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
        
        return 1;
    }
    return 0;
}

void tryExit(Grid grid, int** seen, int i, int j, ExitPoint* exits, int exitsCount, Door* doors, Key* keys, int* minDist, int currDist, Node** doorHead, PlayerPos plPos) {
    // tryExit looks for an exit point to escape the maze, also sees and saves keys and doors on the way
    
    int currentField = grid.grid[i][j];

    if (currentField >= 101 && currentField <= 120)
    {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL)
        {
            error(MemoryError);
        }
        newNode->x = i;
        newNode->y = j;
        newNode->next = *doorHead;
        *doorHead = newNode;
    }

   
    for (int e = 0; e < exitsCount; e++)
    {   
        // if player is on an exit point
        if (i == exits[e].x && j == exits[e].y)
        {
            Node* iter = *doorHead;
            printf("current exit: %d,%d\n", exits[e].x, exits[e].y);
            while (iter != NULL)
            {
                printf("current door: %d,%d\n", iter->x, iter->y);
                iter = iter->next;
            }
            
            int** seen2 = (int**)calloc(grid.rows, sizeof(int));
            if(seen2 == NULL) {
                error(MemoryError);
            }
            for (int i = 0; i < grid.rows; ++i)
            {
                // set columns to the seen rows
                seen2[i] = (int*)calloc(grid.cols, sizeof(int));
                if(seen2[i] == NULL) {
                    error(MemoryError);
                }
            }

            iter = *doorHead;
            int dist = __INT_MAX__;
            int shortestDist = 0;
            int key = 0;
            int currPosX = exits[e].x;
            int currPosY = exits[e].y;
            int newPosX = 0;
            int newPosY = 0;
            if (iter != NULL)
            {
                if (grid.grid[exits[e].x][exits[e].y] == 0)
                {
                    shortestPath(grid, seen2, currPosX, currPosY, grid.grid[(iter)->x][(iter)->y], &dist, 0, &newPosX, &newPosY);
                    shortestDist += dist;
                    currPosX = newPosX;
                    currPosY = currPosY;
                }
            }

            while (iter != NULL)
            {
                dist = __INT_MAX__;
                key = grid.grid[(iter)->x][(iter)->y] - 100;
                shortestPath(grid, seen2, currPosX, currPosY, key, &dist, 0, &newPosX, &newPosY);
                shortestDist += dist;
                currPosX = newPosX;
                currPosY = newPosY;

                iter = iter->next;
            }
            dist = __INT_MAX__;
            shortestPath(grid, seen2, plPos.x, plPos.y, key, &dist, 0, &newPosX, &newPosY);
            shortestDist += dist;

            shortestDist++;
            // if the current distance is shorter than the minimum distance
            // set the minimum distance to current distance
            *minDist = shortestDist < *minDist ? shortestDist : *minDist;
            printf("current shortestDist: %d\n", shortestDist);
            return;
        }
    }    

    // set current position of player to be stepped on
    seen[i][j] = 1;

    // go to bottom field
    if (tE_isSafe(grid, seen, doors, keys, i+1, j)) 
    {
        tryExit(grid, seen, i+1, j, exits, exitsCount, doors, keys, minDist, currDist+1, doorHead, plPos);
    }

    // go to right field
    if (tE_isSafe(grid, seen, doors, keys, i, j+1)) 
    {
        tryExit(grid, seen, i, j+1, exits, exitsCount, doors, keys, minDist, currDist+1, doorHead, plPos);
    }
    // go to top field
    if (tE_isSafe(grid, seen, doors, keys, i-1, j)) 
    {
        tryExit(grid, seen, i-1, j, exits, exitsCount, doors, keys, minDist, currDist+1, doorHead, plPos);
    }
    // go to left field
    if (tE_isSafe(grid, seen, doors, keys, i, j-1)) 
    {
        tryExit(grid, seen, i, j-1, exits, exitsCount, doors, keys, minDist, currDist+1, doorHead, plPos);
    }
    
    // backtracking
    seen[i][j] = 0;
    if(*doorHead != NULL) {
        void* ptr = *doorHead;
        *doorHead = (*doorHead)->next;
        free(ptr);
    }
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

    Node* doorHead = NULL;
    Node* keyHead = NULL;

    tryExit(grid, seen, currentX, currentY, exits, exitsCount, doors, keys, &minDist, 0, &doorHead, plPos);
    
    // if minimum Distance is unchanged it means there is no exit so return 0
    return minDist;
}