#include "placesample.h"

PlaceSample::PlaceSample(bool me, position pos1, position pos2, echantillon ech)
{
    _me = me;
    _pos1 = pos1;
    _pos2 = pos2;
    _ech = ech;
}

void PlaceSample::simulate(GameSimulator& sim)
{
    BoardSimulator& board = _me ? sim.myBoard : sim.oppBoard;
    board.putSample(_pos1, _pos2, _ech);
}

void PlaceSample::undo(GameSimulator& sim)
{
    BoardSimulator& board = _me ? sim.myBoard : sim.oppBoard;
    board.putSample(_pos1, _pos2, echantillon{VIDE,VIDE});
}

void PlaceSample::execute()
{
    placer_echantillon(_pos1, _pos2);
}
