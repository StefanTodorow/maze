#pragma once

typedef enum Errors Errors;
typedef struct PlayerPos PlayerPos;
typedef struct Grid Grid;

struct Grid {
    int rows;
    int cols;
    int** grid;
};

struct PlayerPos {
    int x;
    int y;
};

enum Errors {
    FileError,
    MemoryError,

    Count
};