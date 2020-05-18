#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

#include <vector>
#include "common.h"

class Block {
    static const int NUM_POINT_ = 9;
  public:
    bool isValid() const;
    void rowPrint() const;
    void push_back(Cell *cell);

  private:
    std::vector<Cell*> block_;
};

#endif
