#include "actions_wipeout.h"
#include "prototypes.h"

Wipeout::Wipeout(bool me)
{
    _me = me;
}

void Wipeout::simulate(GameSimulator& sim)
{
    _scoreDiff = 0;
    _catalyserDiff = 0;

    BoardSimulator& board = _me ? sim.myBoard : sim.oppBoard;
    for(std::vector<position> reg : board.getRegions())
    {
        if(reg.size() < 2) continue;
        _scoreDiff += regionGoldValue(reg.size(), board.typeCase(reg[0]));
        _catalyserDiff += regionCatalyserValue(reg.size(), board.typeCase(reg[0]));
    }

    _backupBoard = board.copy();
    board.wipeout();
    if(_me)
    {
        sim.penalty += WIPEOUT_PENALTY;
        sim.myScore += _scoreDiff;
        sim.myCatalyser += _catalyserDiff;
    }
    else
    {
        sim.penalty -= WIPEOUT_PENALTY;
        sim.oppScore += _scoreDiff;
        sim.oppCatalyser += _catalyserDiff;
    }
}

void Wipeout::undo(GameSimulator& sim)
{
    if(_me)
    {
        sim.myBoard.swap(_backupBoard);
        sim.penalty -= WIPEOUT_PENALTY;
        sim.myScore -= _scoreDiff;
        sim.myCatalyser -= _catalyserDiff;
    }
    else
    {
        sim.oppBoard.swap(_backupBoard);
        sim.penalty += WIPEOUT_PENALTY;
        sim.oppScore -= _scoreDiff;
        sim.oppCatalyser -= _catalyserDiff;
    }
}

void Wipeout::execute()
{
    printf("Wipeout volontaire !\n");
    BoardSimulator wipedBoard(moi());
    for(std::vector<position> reg : wipedBoard.getRegions())
    {
        if(reg.size() < 2) continue;
        if(transmuter(reg[0]) != OK) printf("FAIL Wipeout...\n");
    }
}

void Wipeout::debugPrint()
{
    printf("Wipeout\n");
}
