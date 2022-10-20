#include <stdio.h>
#include <stdlib.h>
#include "structs/structs.h"
#include "funcs/funcs.h"
#define DEBUG

#ifdef DEBUG



#endif

int main() {
    FILE* input = openInput("./IO_files/INPUT.txt");

    // set player's position and grid
    PlayerPos plPos = initPlayer(input);
    Grid grid = initGrid(input);

    fclose(input);

    puts("grid");
    printGrid(grid);
    puts("player pos");
    printf("%d %d\n", plPos.x, plPos.y);

    validateGrid(grid);
    validatePlayerPos(plPos, grid);

    int shortest = shortestExit(grid, plPos);
    printf("Shortest exit: %d\n", shortest);

    FILE* output = openOutput("./IO_files/OUTPUT.txt");
    fprintf(output, "%d", shortest);
    fclose(output);

    return 0;
}