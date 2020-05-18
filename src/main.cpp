#include <iostream>
#include <string>
#include <cstring>
#include "board.h"
#include "setup.h"
#include "test.h"

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
    Board board;

    if (argc == 1) {
        int numErase = setLevel();
        board.generate();
        board.randomErase(numErase);
    }
    else if (argc == 3 && !strcmp(argv[1], "-l")) {
        // load saved game progress
        board.load(argv[2]);
    }
    else {
        printHelp();
        return 0;
    }

    board.play();
#endif
    return 0;
}
