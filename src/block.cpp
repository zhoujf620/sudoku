#include <cassert>
#include <iostream>
#include "../include/block.h"

Block::Block() : idx_(0) {}

bool Block::isValid() const {
    assert(NUM_CELL_ == idx_);

    for (int i = 0, num = 0; i < NUM_CELL_; ++i) {
        int value = block_[i]->value;
        if (0 == value) return false;
        if (num & (1 << value)) return false;
        num |= (1 << value);
    }
    return true;
}

void Block::rowPrint() const {
    std::cout << "| ";
    for (int i = 0; i < NUM_CELL_; ++i) {
        Cell *cell  = block_[i];
        if (0 == cell->value)
            std::cout << ' ' << " | ";
        else {
            if (cell->erased)
                std::cout << "\033[32m" << cell->value << "\033[0m" << " | ";
            else
                std::cout << cell->value << " | ";
        }
    }
    std::cout << std::endl;
}

void Block::push_back(Cell *cell) {
    assert(nullptr != cell);
    block_[idx_++] = cell;
}
