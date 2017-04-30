#include "actions_transmute.h"
#include "prototypes.h"

Transmute::Transmute(bool me, std::vector<position> region)
{
    _me = me;
    _region = region;
}

void Transmute::simulate(GameSimulator& sim)
{
    BoardSimulator& board = _me ? sim.myBoard : sim.oppBoard;

    _type = board.typeCase(_region[0]);
    _diffScore = regionGoldValue(_region.size(), _type);
    _diffCatalyser = regionCatalyserValue(_region.size(), _type);

    board.fillRegion(_region, VIDE);

    if(_me)
    {
        if(_region.size() == 1) sim.penalty += ONE_PENALTY;
        if(_region.size() == 2) sim.penalty += TWO_PENALTY;
        sim.myScore += _diffScore;
        sim.myCatalyser += _diffCatalyser;
    }
    else
    {
        if(_region.size() == 1) sim.penalty -= ONE_PENALTY;
        if(_region.size() == 2) sim.penalty -= TWO_PENALTY;
        sim.oppScore += _diffScore;
        sim.oppCatalyser += _diffCatalyser;
    }
}

void Transmute::undo(GameSimulator& sim)
{
    if(_me)
    {
        sim.myBoard.fillRegion(_region, _type);
        if(_region.size() == 1) sim.penalty -= ONE_PENALTY;
        if(_region.size() == 2) sim.penalty -= TWO_PENALTY;
        sim.myScore -= _diffScore;
        sim.myCatalyser -= _diffCatalyser;
    }
    else
    {
        sim.oppBoard.fillRegion(_region, _type);
        if(_region.size() == 1) sim.penalty += ONE_PENALTY;
        if(_region.size() == 2) sim.penalty += TWO_PENALTY;
        sim.oppScore -= _diffScore;
        sim.oppCatalyser -= _diffCatalyser;
    }
}

void Transmute::execute()
{
    if(transmuter(_region[0]) != OK) printf("FAIL Transmute...");
}

void Transmute::debugPrint()
{
    printf("Transmuter (%d,%d)\n", _region[0].ligne, _region[0].colonne);
}
