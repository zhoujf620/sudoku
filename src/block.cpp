#include <cassert>
#include <iostream>
#include "common.h"
#include "block.h"

Block::Block() : count_(0) {}

bool Block::isValid() const {
    assert(NUM_POINT_ == count_);

    for (size_t i = 0; i < count_ - 1; ++i) {
        for (size_t j = i + 1; j < count_; ++j) {
            if (0 == _numbers[i]->value || 0 == _numbers[j]->value)
                continue;

            if (_numbers[i]->value == _numbers[j]->value)
                return false;
        }
    }

    return true;
}

bool Block::isFull() const {
    for (int i = 0; i < count_; ++i) {
        point_value_t *p_point_value = _numbers[i];
        if (nullptr == p_point_value || 0 == p_point_value->value)
            return false;
    }
    return true;
}

void Block::rowPrint() const {
    std::cout << "| ";
    for (int i = 0; i < count_; ++i) {
        auto number = *(_numbers[i]);
        if (0 == number.value)
            std::cout << ' ' << " | ";
        else {
            if (number.state == State::ERASED)
                std::cout << "\033[32m" << number.value << "\033[0m" << " | ";
            else
                std::cout << number.value << " | ";
        }
    }
    std::cout << std::endl;
}

void Block::push_back(point_value_t *point) {
    assert(nullptr != point);
    _numbers[count_++] = point;
}
