#ifndef _SUDOKU_COMMON_H_
#define _SUDOKU_COMMON_H_

static const unsigned int UNSELECTED = 0;

enum class GameLevel : int {
    EASY = 1,
    NORMAL,
    HARD
};

enum class State : int {
    INITED = 0,
    ERASED,
};

typedef struct {
    int x;
    int y;
} Point;

using point_value_t = struct point_value_t {
    int value;
    State state;
};

class CPointSort {
  public:
    bool operator()(const Point &lhs, const Point &rhs) const {
        if ((lhs.x == rhs.x) && (lhs.y == rhs.y))
            return false;
        else
            return true;
    }
};

#endif
