#include "actions_wipeout.h"

Wipeout::Wipeout(bool me)
{
    _me = me;
}

void Wipeout::simulate(GameSimulator& sim)
{
    if(_me)
    {
        _backupBoard = sim.myBoard.copy();
        sim.myBoard.wipeout();
    }
    else
    {
        _backupBoard = sim.oppBoard;
        sim.oppBoard.wipeout();
    }
}

void Wipeout::undo(GameSimulator& sim)
{
    if(_me) sim.myBoard.swap(_backupBoard);
    else sim.oppBoard.swap(_backupBoard);
}

void Wipeout::execute()
{
    printf("Wipeout volontaire !\n");
}
