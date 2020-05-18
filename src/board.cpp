#include <assert.h>
#include <memory.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../include/common.h"
#include "../include/board.h"
#include "../include/utility.inl"

// TODO 单例模式
Board::Board(int numErase) : numErase_(numErase) {
    srand(time(NULL));
    memset(board_, 0, sizeof(board_));

    for (int col = 0; col < BOARD_SIZE_; ++col) {
        Block col_block;

        for (int row = 0; row < BOARD_SIZE_; ++row) {
            col_block.push_back(board_ + row * BOARD_SIZE_ + col);
        }
        col_block_[col] = col_block;
    }

    for (int row = 0; row < BOARD_SIZE_; ++row) {
        Block row_block;

        for (int col = 0; col < BOARD_SIZE_; ++col) {
            row_block.push_back(board_ + row * BOARD_SIZE_ + col);
        }
        row_block_[row] = row_block;
    }

    for (int sub_idx = 0; sub_idx < BOARD_SIZE_; ++sub_idx) {
        Block sub_block;

        int xy_begin = sub_idx / 3 * 27 + sub_idx % 3 * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sub_block.push_back(board_ + xy_begin + i * BOARD_SIZE_ + j);
            }
        }
        sub_block_[sub_idx / 3][sub_idx % 3] = sub_block;
    }
}

Board::~Board() {}

void Board::generate() {
    static char board_case[10][10] = {
        "ighcabfde",
        "cabfdeigh",
        "fdeighcab",
        "ghiabcdef",
        "abcdefghi",
        "defghiabc",
        "higbcaefd",
        "bcaefdhig",
        "efdhigbca"
    };

    std::vector<char> chars{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    random_shuffle(chars);
    
    std::unordered_map<char, int> char2int;
    for (int i = 0; i < chars.size(); ++i) char2int[chars[i]] = i + 1;
    
    for (int row = 0; row < BOARD_SIZE_; ++row) {
        for (int col = 0; col < BOARD_SIZE_; ++col) {
            Coord coord = {row, col};
            char ch = board_case[row][col];
            __setValue(coord, char2int[ch]);
        }
    }

    assert(__isValid());
    __randomErase();
    __show();
    return;
}

void Board::play() {

    char key = '\0';
    while (1) {
        key = getch();
        if (key >= '0' && key <= '9') {
            Cell* cell = &board_[cur_coord_.x + cur_coord_.y * BOARD_SIZE_];
            if (!cell->erased) {
                std::cout << "this number can't be modified." << std::endl;
            } else {
                cell->value = key - '0';
                __show();
                continue;
            }
        }
        switch (key) {
            case 0x1B: {    // ESC  
                std::cout << "quit game ? [Y/N]" << std::endl;
                std::string strInput;
                std::cin >> strInput;
                if (strInput[0] == 'y' || strInput[0] == 'Y') {
                    exit(0);
                } else {
                    std::cout << "continue." << std::endl;
                    break;
                }
            }
            case 0x61: // a
                cur_coord_.x = (cur_coord_.x - 1) < 0 ? 0 : cur_coord_.x - 1;
                __show();
                break;
            case 0x64: // d
                cur_coord_.x = (cur_coord_.x + 1) > 8 ? 8 : cur_coord_.x + 1;
                __show();
                break;
            case 0x73: // s
                cur_coord_.y = (cur_coord_.y + 1) > 8 ? 8 : cur_coord_.y + 1;
                __show();
                break;
            case 0x77: // w
                cur_coord_.y = (cur_coord_.y - 1) < 0 ? 0 : cur_coord_.y - 1;
                __show();
                break;
            case 0x0D: {    // enter
                if (!__isComplete()) {
                    std::cout << "Not completed." << std::endl;
                } else if (__isValid()) {
                    std::cout << "Congrats! You win!" << std::endl;
                    getchar();
                    exit(0);
                } else {
                    std::cout << "Soory, wrong answer." << std::endl;
                }
                break;
            }
            default:
                break;
        }
    }
}

void Board::__show() const {
    cls();

    __printUnderline();
    for (int row = 0; row < BOARD_SIZE_; ++row) {
        row_block_[row].rowPrint();
        __printUnderline(row);
    }
}

void Board::__printUnderline(int line_no) const {
    std::string underline;

    for (int col = 0; col < BOARD_SIZE_; ++col) {
        if (cur_coord_.y == line_no && cur_coord_.x == col)
            underline += "--^-";
        else
            underline += "----";
    }
    underline += '-';

    for (int i = 0; i < 4; i++) {
        underline[i * 12] = ((line_no + 1) % 3 == 0) ? '+' : '|';
    }

    std::cout << underline << std::endl;
}

void Board::__randomErase() {

    /* Reservoir Sampling */
    std::vector<int> nums(BOARD_SIZE_ * BOARD_SIZE_);
    std::iota(nums.begin(), nums.end(), 0);

    std::vector<int> samples(nums.begin(), nums.begin() + numErase_);
    for (int i = numErase_; i < nums.size(); ++i) {
        int j = rand() % (i + 1);
        if (j < numErase_)
            samples[j] = nums[i];
    }
    for (int i = 0; i < numErase_; ++i) {
        board_[samples[i]].erased = true;
        board_[samples[i]].value = 0;
    }
}

bool Board::__isComplete() const {
    for (int i = 0; i < BOARD_SIZE_ * BOARD_SIZE_; ++i) {
        if (board_[i].value == 0)
            return false;
    }
    return true;
}

bool Board::__isValid() const {
    if (!__isComplete()) return false;

    for (int i = 0; i < BOARD_SIZE_; ++i) {
        if (!row_block_[i].isValid() ||
            !col_block_[i].isValid() ||
            !sub_block_[i / 3][i % 3].isValid()) 
            return false;
    }
    return true;
}

void Board::__setValue(const Coord& coord, const int value) {
    board_[coord.x + coord.y * BOARD_SIZE_].value = value;
}

void Board::__setValue(const int value) {
    this->__setValue(cur_coord_, value);
}
