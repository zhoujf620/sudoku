#ifndef _SUDOKU_UTILITY_INL_
#define _SUDOKU_UTILITY_INL_

#include <random>
#include <algorithm>

template<typename T>
inline void random_shuffle(std::vector<T> &v){
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
}

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <cstdio>
inline char getch(void) {
    struct termios newt, oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    cfmakeraw(&newt);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif


inline void cls(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

#endif
