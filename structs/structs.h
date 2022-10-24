#pragma once

typedef enum Errors Errors;
typedef struct PlayerPos PlayerPos;
typedef struct Grid Grid;
typedef struct ExitPoint ExitPoint;
typedef struct Door Door;
typedef struct Key Key;
typedef struct Node Node;

struct Grid {
    int rows;
    int cols;
    int** grid;
};

struct PlayerPos {
    int x;
    int y;
};

struct ExitPoint {
    int x;
    int y;
};

struct Door {
    int x;
    int y;
    int isReachable;
    int isUsed;
};

struct Key {
    int x;
    int y;
    int isReachable;
    int isUsed;
};

struct Node {
    int x;
    int y;
    Node* next;
};

enum Errors {
    FileError,
    MemoryError,
    GridSizeError,
    GridFieldError,
    PlayerPosError,
    NoExitError,

    Count
};