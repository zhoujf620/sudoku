#include <cmath>
#include <iostream>
#include <fstream>
#include <memory.h>
#include <unordered_map>
#include <vector>
#include "common.h"
#include "board.h"
#include "utility.inl"

Board::Board() {
    __init();
}

Board::~Board() {}

// 一个场景可以多次被初始化
void Board::generate() {
    // XXX: pseudo random
    static char map_pattern[10][10] = {
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

    std::vector<char> v = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};

    // 产生字母到数字的随机映射
    std::unordered_map<char, int> hash_map;
    for (int i = 1; i <= 9; ++i) {
        int r = random(0, v.size() - 1);
        hash_map[v[r]] = i;
        v.erase(v.begin() + r);
    }

    // 填入场景
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            Point point = {row, col};
            char key = map_pattern[row][col];
            __setValue(point, hash_map[key]);
        }
    }

    assert(isComplete());
    return;
}

void Board::show() const {
    cls();

    __printUnderline();

    for (int row = 0; row < max_column_; ++row) {
        Block block = row_block_[row];
        block.print();
        __printUnderline(row);
    }
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

bool Board::setPointValue(const Point &stPoint, const int nValue) {
    auto point = map_[stPoint.x + stPoint.y * 9];
    if (State::ERASED == point.state) {
        cur_point_ = stPoint;
        __setValue(nValue);
        return true;
    }
    else
        return false;
}

Point Board::getCurPoint() {
    return cur_point_;
}

// 选择count个格子清空
void Board::eraseRandomGrids(const int count) {
    point_value_t p = {UNSELECTED, State::ERASED};

    std::vector<int> v(81);
    for (int i = 0; i < 81; ++i) {
        v[i] = i;
    }

    for (int i = 0; i < count; ++i) {
        int r = random(0, v.size() - 1);
        map_[v[r]] = p;
        v.erase(v.begin() + r);
    }
}

bool Board::isComplete() {
    // 任何一个block未被填满，则肯定未完成
    for (size_t i = 0; i < 81; ++i) {
        if (map_[i].value == UNSELECTED)
            return false;
    }

    // 同时block里的数字还要符合规则
    for (size_t row = 0; row < 9; ++row) {
        for (size_t col = 0; col < 9; ++col) {
            if (!row_block_[row].isValid() || 
                !col_block_[col].isValid() || 
                !sub_block_[row / 3][col / 3].isValid())
                return false;
        }
    }

    return true;
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
            case 0x1B: {// ESC  
                std::cout << "quit game ? [Y/N]" << std::endl;
                std::string strInput;
                std::cin >> strInput;
                if (strInput[0] == 'y' || strInput[0] == 'Y') {
                    std::cout << "do you want to save the game progress ? [Y/N]" << std::endl;
                    std::cin >> strInput;
                    if (strInput[0] == 'y' || strInput[0] == 'Y') {
                        std::cout << "input path of the progress file: ";
                        std::cin >> strInput;
                        save(strInput.c_str());
                    }
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

void Board::save(const char *filename) {
    std::fstream fs;
    // TODO: check whether the file has existed
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    // save map_
    for (int i = 0; i < 81; i++) {
        fs << map_[i].value << ' ' << static_cast<int>(map_[i].state) << std::endl;
    }

    // save cur_point_
    fs << cur_point_.x << ' ' << cur_point_.y << std::endl;

    // save vCommand_
    fs << vCommand_.size() << std::endl;
    for (CCommand command : vCommand_) {
        Point point = command.getPoint();
        fs << point.x << ' ' << point.y << ' '
           << command.getPreValue() << ' '
           << command.getCurValue() << std::endl;
    }

    fs.close();
}

void Board::load(const char *filename) {
    std::fstream fs;
    // TODO: check whether the file has existed
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    // load map_
    for (int i = 0; i < 81; i++) {
        int tmpState;
        fs >> map_[i].value >> tmpState;
        map_[i].state = static_cast<State>(tmpState);
    }

    // load cur_point_
    fs >> cur_point_.x >> cur_point_.y;

    // load vCommand_
    int commandSize;
    fs >> commandSize;
    for (int i = 0; i < commandSize; i++) {
        Point point;
        int preValue, curValue;
        fs >> point.x >> point.y >> preValue >> curValue;
        vCommand_.emplace_back(this, point, preValue, curValue);
    }
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
    memset(map_, UNSELECTED, sizeof(map_));

    // column_block 所有列
    for (int col = 0; col < max_column_; ++col) {
        Block column_block;

        for (int row = 0; row < max_column_; ++row) {
            column_block.push_back(map_ + row * 9 + col);
        }
        col_block_[col] = column_block;
    }

    // row_block 所有行
    for (int row = 0; row < max_column_; ++row) {
        Block row_block;

        for (int col = 0; col < max_column_; ++col) {
            row_block.push_back(map_ + row * 9 + col);
        }
        row_block_[row] = row_block;
    }

    // sub_block 所有九宫格, [行][列]
    for (int block_index = 0; block_index < max_column_; ++block_index) {
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

void Board::__setValue(const Point& p, const int value) {
    map_[p.x + p.y * 9].value = value;
}

void Board::__setValue(const int value) {
    auto p = cur_point_;
    this->__setValue(p, value);
}
