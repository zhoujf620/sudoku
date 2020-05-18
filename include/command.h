#ifndef _SUDOKU_COMMAND_H_
#define _SUDOKU_COMMAND_H_

#include <memory>
#include "common.h"

class Board;
class CCommand {
public:
    CCommand(Board* pOwner);
    CCommand(Board *pOwner, const Coord &point, int preValue, int curValue);
    CCommand(const CCommand &);
    ~CCommand();

    bool execute(int nInputValue);
    void undo();
    Coord getPoint() { return _stPoint; }
    int getPreValue() { return _nPreValue; }
    int getCurValue() { return _nCurValue; }
    void setPoint(const Coord &point) { _stPoint = point; }
    void setPreValue(int preValue) { _nPreValue = preValue; }
    void setCurValue(int curValue) { _nCurValue = curValue; }

private:
    Board* _pOwner;
    Coord _stPoint;
    int _nPreValue;
    int _nCurValue;  // actually the member is never used
};

#endif