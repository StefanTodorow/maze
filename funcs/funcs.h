#pragma once

#include <stdio.h>
#include "C:\Users\stefa\OneDrive\Desktop\maze\maze\structs\structs.h"

void error(Errors err);

FILE* openInput(char* fName);

PlayerPos initPlayer(FILE* input);

Grid initGrid(FILE* input);
void printGrid(Grid grid);
void freeGrid(Grid grid);

void validateGrid(Grid grid);
void validatePlayerPos(PlayerPos pP, Grid grid);

int findAllExitPoints(Grid grid, ExitPoint* exitPoints);

int isSafe(Grid grid, int** seen, int x, int y);
int shortestExit(Grid grid, PlayerPos plPos, ExitPoint* exits, int exitsCount);
void tryPaths(Grid grid, int** seen, int i, int j, ExitPoint* exits, int exitsCount, int* minDist, int currDist);
