#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

#include "common.h"

class Block {
    static const int NUM_POINT_ = 9;
  public:
    Block();
    bool isValid() const;
    bool isFull() const;
    void rowPrint() const;
    void push_back(point_value_t *point);

  private:
    int count_;
    point_value_t *_numbers[NUM_POINT_];
};

#endif
