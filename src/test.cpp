#include "../include/board.h"
#include "../include/test.h"

void test_case() {
    Board* board = Board::getBoard(10);
    board->generate();
}
