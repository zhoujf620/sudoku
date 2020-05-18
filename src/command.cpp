#include "../include/board.h"
#include "../include/command.h"

CCommand::CCommand(Board *pOwner) : _pOwner(pOwner)
{}

CCommand::CCommand(Board *pOwner, const Coord &point, int preValue, int curValue)
    : _pOwner(pOwner), _stPoint(point), _nPreValue(preValue), _nCurValue(curValue) {}

CCommand::CCommand(const CCommand &rhs)
: _pOwner(rhs._pOwner)
, _stPoint(rhs._stPoint)
, _nPreValue(rhs._nPreValue)
, _nCurValue(rhs._nCurValue)
{}

CCommand::~CCommand(){}

bool CCommand::execute(int nInputValue)
{
    if (!_pOwner)
        return false;

    _stPoint = _pOwner->getCurPoint();
    return _pOwner->setCurValue(nInputValue, _nPreValue);
}

void CCommand::undo()
{
    if (_pOwner)
    {
        _pOwner->setPointValue(_stPoint, _nPreValue);
    }

    return;
}
