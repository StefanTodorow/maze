#pragma once

#include <stdio.h>
#include "C:\Users\stefa\OneDrive\Desktop\maze\maze\structs\structs.h"

void error(Errors err);

FILE* openInput(char* fName);

PlayerPos initPlayer(FILE* input);

Grid initGrid(FILE* input);
void printGrid(Grid grid);
void freeGrid(Grid grid);