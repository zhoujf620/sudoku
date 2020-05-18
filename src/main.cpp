#include <iostream>
#include "../include/board.h"
#include "../include/setup.h"
#include "../include/test.h"

#define _TEST_ 0

static void printHelp() {
    std::cout << std::endl;
    std::cout << "sudoku - a little game in command line" << std::endl
              << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t sudoku [-l <progressFile>]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -l <path> \t specify path of progress file to load, optional." << std::endl
              << std::endl;
}

int main(int argc, char **argv) {
#if _TEST_
    test_case();
    getchar();
#else
    if (argc == 1) {
        Board board(setLevel());
        board.generate();
        board.play();
    } else {
        printHelp();
        return 0;
    }
#endif
    return 0;
}
