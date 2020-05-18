#ifndef _SUDOKU_COMMON_H_
#define _SUDOKU_COMMON_H_

enum class GameLevel : int {
    EASY = 1,
    NORMAL,
    HARD
};
typedef struct {
    int x;
    int y;
} Coord;

enum class State : int {
    INITED = 0,
    ERASED,
};

typedef struct {
    int value;
    State state;
} Cell;

#endif
