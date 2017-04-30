#include "actions_catalyse.h"
#include "actions_transmute.h"
#include "gamesimulator.h"

Catalyse::Catalyse(bool fromMe, bool toMe, position pos, case_type dest_type)
{
    _fromMe = fromMe;
    _toMe = toMe;
    _pos = pos;
    _type = dest_type;
}

void Catalyse::simulate(GameSimulator& sim)
{
    if(_toMe)
    {
        _prevType = sim.myBoard.typeCase(_pos);
        sim.myBoard.setCase(_pos, _type);
    }
    else
    {
        _prevType = sim.oppBoard.typeCase(_pos);
        sim.oppBoard.setCase(_pos, _type);
    }

    if(_fromMe) sim.myCatalyser--;
    else sim.oppCatalyser--;
}

void Catalyse::undo(GameSimulator& sim)
{
    if(_toMe) sim.myBoard.setCase(_pos, _prevType);
    else sim.oppBoard.setCase(_pos, _prevType);

    if(_fromMe) sim.myCatalyser++;
    else sim.oppCatalyser++;
}

void Catalyse::execute()
{
    erreur err = catalyser(_pos, _toMe ? moi() : adversaire(), _type);
    if(err != OK) printf("FAIL Catalyse (%d)...", err);
}

void Catalyse::debugPrint()
{
    if(_toMe) printf("Catalyse Me (%d,%d)->%d\n", _pos.ligne, _pos.colonne, _type);
    else printf("Catalyse Opp (%d,%d)->%d\n", _pos.ligne, _pos.colonne, _type);
}

Action* Catalyse::defenseReaction(GameSimulator& game)
{
    if(!_toMe) return nullptr;
    return new Transmute(true, game.myBoard.regionOf(_pos));
}
