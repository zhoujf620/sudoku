#ifndef _SUDOKU_COMMON_H_
#define _SUDOKU_COMMON_H_

enum class GameLevel : int {
    TEST = 0,
    EASY,
    NORMAL,
    HARD
};
typedef struct {
    int x;
    int y;
} Coord;

typedef struct {
    int value;
    bool erased;
} Cell;

#endif
