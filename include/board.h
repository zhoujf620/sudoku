#ifndef _SUDOKU_BOARD_H_
#define _SUDOKU_BOARD_H_

#include <vector>
#include "common.h"
#include "block.h"
#include "command.h"

class Board {
    static const int BOARD_SIZE_ = 9;
  public:
    Board();
    virtual ~Board();

    void generate();
    void randomErase(const int count);
    void show() const;
    void play();
    bool isComplete();
    bool isValid();

    bool setCurValue(const int nCurValue, int& nLastValue);
    bool setPointValue(const Coord&, const int nValue);
    Coord getCurPoint();

  private:
    void __init(); // 将每个格子的指针放到block里面
    void __setValue(const int);
    void __setValue(const Coord &, const int);
    void __printUnderline(int line_no = -1) const;

    Coord cur_point_{0, 0};
    Block row_block_[9];
    Block col_block_[9];
    Block sub_block_[3][3];
    Cell map_[81];
    std::vector<CCommand> vCommand_;
};

#endif
