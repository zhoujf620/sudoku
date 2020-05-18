#ifndef _SUDOKU_BOARD_H_
#define _SUDOKU_BOARD_H_

#include "common.h"
#include "block.h"

class Board {
    static const int BOARD_SIZE_ = 9;
  public:
    Board(int);
    virtual ~Board();

    void generate();
    void play();

  private:
    void __show() const;
    void __printUnderline(int line_no = -1) const;
    void __randomErase();
    bool __isComplete() const;
    bool __isValid() const;
    void __setValue(const int);
    void __setValue(const Coord &, const int);

    const int numErase_;
    Coord cur_coord_{0, 0};
    Block row_block_[9];
    Block col_block_[9];
    Block sub_block_[3][3];
    Cell board_[81];
};

#endif
