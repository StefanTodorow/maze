#include <stdio.h>
#include <stdlib.h>
#include "structs/structs.h"
#include "funcs/funcs.h"

int main() {
    FILE* input = openInput("INPUT.txt");

    PlayerPos plPos = initPlayer(input);
    Grid grid = initGrid(input);

    fclose(input);

    puts("grid");
    printGrid(grid);
    puts("player pos");
    printf("%d %d\n", plPos.x, plPos.y);

    freeGrid(grid);

    return 0;
}