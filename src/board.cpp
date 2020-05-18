#include <iostream>
#include <memory.h>
#include <unordered_map>
#include <vector>
#include "../include/common.h"
#include "../include/board.h"
#include "../include/utility.inl"

Board::Board() {
    __init();
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
    
    // TODO: shuffle
    std::unordered_map<char, int> char2int;
    // 产生字母到数字的随机映射
    for (int i = 1; i <= 9; ++i) {
        int idx = random(0, chars.size() - 1);
        char2int[chars[idx]] = i;
        chars.erase(chars.begin() + idx);
    }

    // 填入场景
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            Coord point = {row, col};
            char key = board_case[row][col];
            __setValue(point, char2int[key]);
        }
    }

    assert(isComplete());
    return;
}

void Board::randomErase(const int count) {
    Cell cell = {0, State::ERASED};

    // TODO 蓄水池算法
    std::vector<int> v(81);
    for (int i = 0; i < 81; ++i) {
        v[i] = i;
    }

    for (int i = 0; i < count; ++i) {
        int r = random(0, v.size() - 1);
        map_[v[r]] = cell;
        v.erase(v.begin() + r);
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
                if (isComplete()) {
                    std::cout << "congratulation! you win!" << std::endl;
                    getchar();
                    exit(0);
                } else {
                    std::cout << "sorry, not completed." << std::endl;
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
        if (map_[i].value == 0)
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
    auto point = map_[cur_point_.x + cur_point_.y * 9];
    if (point.state == State::ERASED) {
        nLastValue = point.value;
        __setValue(nCurValue);
        return true;
    }
    else
        return false;
}

bool Board::setPointValue(const Coord &stPoint, const int nValue) {
    auto point = map_[stPoint.x + stPoint.y * 9];
    if (State::ERASED == point.state) {
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

void Board::__init() {
    memset(map_, 0, sizeof(map_));

    // column_block 所有列
    for (int col = 0; col < BOARD_SIZE_; ++col) {
        Block col_block;

        for (int row = 0; row < BOARD_SIZE_; ++row) {
            col_block.push_back(map_ + row * 9 + col);
        }
        col_block_[col] = col_block;
    }

    // row_block 所有行
    for (int row = 0; row < BOARD_SIZE_; ++row) {
        Block row_block;

        for (int col = 0; col < BOARD_SIZE_; ++col) {
            row_block.push_back(map_ + row * 9 + col);
        }
        row_block_[row] = row_block;
    }

    // sub_block 所有九宫格, [行][列]
    for (int block_index = 0; block_index < BOARD_SIZE_; ++block_index) {
        Block sub_block;

        int xy_begin = block_index / 3 * 27 + block_index % 3 * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sub_block.push_back(map_ + xy_begin + i * 9 + j);
            }
        }
        sub_block_[block_index / 3][block_index % 3] = sub_block;
    }

    return;
}

void Board::__setValue(const Coord& p, const int value) {
    map_[p.x + p.y * 9].value = value;
}

void Board::__setValue(const int value) {
    auto p = cur_point_;
    this->__setValue(p, value);
}
