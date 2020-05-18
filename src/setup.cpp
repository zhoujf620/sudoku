#include <iostream>
#include <string>
#include "../include/common.h"
#include "../include/utility.inl"

// return number of grids to be erased
int setLevel() {
    cls();

    std::string cmd;
    while (1) {
        std::cout << "设置难度：1简单 2普通 3困难" << std::endl;
        std::cin >> cmd;
        GameLevel level = static_cast<GameLevel>(std::stoi(cmd));

        switch (level) {
            case GameLevel:: TEST:
                return 1;
            case GameLevel::EASY:
                return 20;
            case GameLevel::NORMAL:
                return 35;
            case GameLevel::HARD:
                return 50;
            default:
                std::cout << "输入错误！" << std::endl;
                continue;
        }
    }

    assert(0);
    return 0;
}
