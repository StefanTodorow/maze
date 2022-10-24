#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\stefa\OneDrive\Desktop\maze\maze\structs\structs.h"

//validation.c
void error(Errors err);
void validateGrid(Grid grid);
void validatePlayerPos(PlayerPos pP, Grid grid);

//IO_funcs.c
FILE* openInput(char* fName);
FILE* openOutput(char* fName);
void printGrid(Grid grid);

//initialization.c
PlayerPos initPlayer(FILE* input);
Grid initGrid(FILE* input);
void initExitPoints(Grid grid, ExitPoint* exitPoints);
int countExitPoints(Grid grid);

//main_funcs.c
int shortestExit(Grid grid, PlayerPos plPos);

void tryExit(Grid grid, int** seen, int i, int j, ExitPoint* exits, int exitsCount, Door* doors, Key* keys, int* minDist, int currDist, Node** doorHead, PlayerPos plPos);
int tE_isSafe(Grid grid, int** seen, Door* doors, Key* keys, int x, int y);

void shortestPath(Grid grid, int** seen, int i, int j, int key, int* shortestP, int currDist, int* newX, int* newY);
int sP_isSafe(Grid grid, int** seen, int x, int y);