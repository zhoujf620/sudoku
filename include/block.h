#ifndef _SUDOKU_BLOCK_H_
#define _SUDOKU_BLOCK_H_

class Block {
    static const int MAX_COUNT_ = 9;
  public:
    Block();
    bool isValid() const;
    bool isFull() const;
    void print() const;
    void push_back(point_value_t *point);

  private:
    int count_;
    point_value_t *_numbers[MAX_COUNT_];
};

#endif