#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

#include "common.h"

class Block {
    static const int NUM_CELL_ = 9;
  public:
    Block();
    bool isValid() const;
    void rowPrint() const;
    void push_back(Cell *cell);

  private:
    int idx_;
    Cell* block_[NUM_CELL_];
};

#endif
