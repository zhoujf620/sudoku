#ifndef _SUDOKU_BOARD_H_
#define _SUDOKU_BOARD_H_

#include <vector>
#include "common.h"
#include "block.h"
#include "command.h"

class Board {
  public:
    Board();
    virtual ~Board();

    void generate();
    void show() const;

    bool setCurValue(const int nCurValue, int& nLastValue);
    bool setPointValue(const Point&, const int nValue);
    Point getCurPoint();

    void eraseRandomGrids(const int count);
    bool isComplete();

    void play();
    void save(const char *filename);
    void load(const char *filename);

  private:
    void __init(); // 将每个格子的指针放到block里面
    void __setValue(const int);
    void __setValue(const Point &, const int);
    void __printUnderline(int line_no = -1) const;

    const int max_column_ = 9;
    Point cur_point_{0, 0};
    Block row_block_[9];
    Block col_block_[9];
    Block sub_block_[3][3];
    point_value_t map_[81];
    std::vector<CCommand> vCommand_;
};

#endif