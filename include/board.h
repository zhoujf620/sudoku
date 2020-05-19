#ifndef _SUDOKU_BOARD_H_
#define _SUDOKU_BOARD_H_

#include "common.h"
#include "block.h"

class Board {
  public:
    static Board* getBoard(int);
    void generate();
    void play();

  private:
    Board(int);
    static Board *instance_;

    void __show() const;
    void __printUnderline(int line_no = -1) const;
    void __randomErase();
    bool __isComplete() const;
    bool __isValid() const;
    void __setValue(const int);
    void __setValue(const Coord &, const int);

    static const int BOARD_SIZE_ = 9;
    const int numErase_;
    Coord cur_coord_{0, 0};
    Block row_block_[9];
    Block col_block_[9];
    Block sub_block_[3][3];
    Cell board_[81];
};

#endif
