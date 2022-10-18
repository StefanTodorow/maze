#include <stdio.h>
#include <stdlib.h>
#include "structs/structs.h"
#include "funcs/funcs.h"
#define DEBUG

#ifdef DEBUG



#endif

int main() {
    FILE* input = openInput("INPUT.txt");

    PlayerPos plPos = initPlayer(input);
    Grid grid = initGrid(input);

    fclose(input);

    puts("grid");
    printGrid(grid);
    puts("player pos");
    printf("%d %d\n", plPos.x, plPos.y);

    validateGrid(grid);
    validatePlayerPos(plPos, grid);

    // allocate memory for max possible exits
    ExitPoint* exits = (ExitPoint*)malloc(192 * sizeof(ExitPoint));
    if(exits == NULL) {
        error(MemoryError);
    }

    // count all possible exits
    int exitsCounter = findAllExitPoints(grid, exits);

    int shortest = shortestExit(grid, plPos, exits, exitsCounter);
    printf("Shortest exit: %d\n", shortest);

    FILE* output = openOutput("OUTPUT.txt");
    fprintf(output, "%d", shortest);
    fclose(output);

    free(exits);
    freeGrid(grid);

    return 0;
}