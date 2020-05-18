#include <iostream>
#include <memory.h>
#include <unordered_map>
#include <vector>
#include "../include/common.h"
#include "../include/board.h"
#include "../include/utility.inl"

// TODO 单例模式
Board::Board() {
    memset(board_, 0, sizeof(board_));

    for (int col = 0; col < BOARD_SIZE_; ++col) {
        Block col_block;

        for (int row = 0; row < BOARD_SIZE_; ++row) {
            col_block.push_back(board_ + row * 9 + col);
        }
        col_block_[col] = col_block;
    }

    for (int row = 0; row < BOARD_SIZE_; ++row) {
        Block row_block;

        for (int col = 0; col < BOARD_SIZE_; ++col) {
            row_block.push_back(board_ + row * 9 + col);
        }
        row_block_[row] = row_block;
    }

    for (int sub_idx = 0; sub_idx < BOARD_SIZE_; ++sub_idx) {
        Block sub_block;

        int xy_begin = sub_idx / 3 * 27 + sub_idx % 3 * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sub_block.push_back(board_ + xy_begin + i * 9 + j);
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

    assert(isValid());
    return;
}

void Board::randomErase(const int numErase) {

    /* Reservoir Sampling */
    std::vector<int> nums(81);
    std::iota(nums.begin(), nums.end(), 0);

    std::vector<int> samples(nums.begin(), nums.begin() + numErase);
    for (int i = numErase; i < nums.size(); ++i) {
        int j = rand() % (i + 1);
        if (j < numErase)
            samples[j] = nums[i];
    }

    for (int i = 0; i < numErase; ++i) {
        board_[samples[i]].erased = true;
        board_[samples[i]].value = 0;
    }
}

void Board::show() const {
    cls();

    __printUnderline();
    for (int row = 0; row < BOARD_SIZE_; ++row) {
        Block block = row_block_[row];
        block.rowPrint();
        __printUnderline(row);
    }
}

void Board::play() {
    show();

    char key = '\0';
    while (1) {
        key = getch();
        if (key >= '0' && key <= '9') {
            CCommand oCommand(this);
            if (!oCommand.execute(key - '0')) {
                std::cout << "this number can't be modified." << std::endl;
            } else {
                vCommand_.push_back(std::move(oCommand));  // XXX: move without move constructor
                show();
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
            case 0x75: {    // u
                if (vCommand_.empty()) {
                    std::cout << "no more action to undo." << std::endl;
                } else {
                    CCommand &oCommand = vCommand_.back();
                    oCommand.undo();
                    vCommand_.pop_back();
                    show();
                }
                break;
            }
            case 0x61: // a
                cur_point_.x = (cur_point_.x - 1) < 0 ? 0 : cur_point_.x - 1;
                show();
                break;
            case 0x64: // d
                cur_point_.x = (cur_point_.x + 1) > 8 ? 8 : cur_point_.x + 1;
                show();
                break;
            case 0x73: // s
                cur_point_.y = (cur_point_.y + 1) > 8 ? 8 : cur_point_.y + 1;
                show();
                break;
            case 0x77: // w
                cur_point_.y = (cur_point_.y - 1) < 0 ? 0 : cur_point_.y - 1;
                show();
                break;
            case 0x0D: {    // enter
                if (!isComplete()) {
                    std::cout << "Not completed." << std::endl;
                } else if (isValid()) {
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

bool Board::isComplete() {
    for (int i = 0; i < 81; ++i) {
        if (board_[i].value == 0)
            return false;
    }
    return true;
}

bool Board::isValid() {
    for (int i = 0; i < 81; ++i) {
        if (board_[i].value == 0)
            return false;
    }

    for (int i = 0; i < 9; ++i) {
        if (!row_block_[i].isValid() ||
            !col_block_[i].isValid() ||
            !sub_block_[i / 3][i % 3].isValid()) 
            return false;
    }
    return true;
}

bool Board::setCurValue(const int nCurValue, int &nLastValue) {
    auto point = board_[cur_point_.x + cur_point_.y * 9];
    if (point.erased) {
        nLastValue = point.value;
        __setValue(nCurValue);
        return true;
    }
    else
        return false;
}

bool Board::setPointValue(const Coord &stPoint, const int nValue) {
    auto point = board_[stPoint.x + stPoint.y * 9];
    if (point.erased) {
        cur_point_ = stPoint;
        __setValue(nValue);
        return true;
    }
    else
        return false;
}

Coord Board::getCurPoint() {
    return cur_point_;
}

void Board::__printUnderline(int line_no) const {
    std::string underline;

    for (int colunm = 0; colunm < 9; ++colunm) {
        if (cur_point_.y == line_no && cur_point_.x == colunm)
            underline += "--^-";
        else
            underline += "----";
    }
    underline += '-';

    for (int i = 0; i < 4; i++) {
        underline[i * 12] = ((line_no + 1) % 3 == 0) ? '+' : '|';
    }

    std::cout << underline.c_str() << std::endl;
}

void Board::__setValue(const Coord& coord, const int value) {
    board_[coord.x + coord.y * 9].value = value;
}

void Board::__setValue(const int value) {
    auto p = cur_point_;
    this->__setValue(p, value);
}
