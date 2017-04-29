#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "boardsimulator.h"

struct GameSimulator
{
    GameSimulator() : myBoard(moi()), oppBoard(adversaire())
    {
        myScore = 0;
        oppScore = 0;
        myCatalyser = 0;
        oppCatalyser = 0;
    }

    int gamePotential(bool me, bool wipeout = false) const
    {
        if(me) return (wipeout ? 0 : myBoard.boardPotential()) + myScore + myCatalyser;
        else return (wipeout ? 0 : oppBoard.boardPotential()) + oppScore + oppCatalyser;
    }

    BoardSimulator myBoard;
    BoardSimulator oppBoard;

    int myScore;
    int oppScore;

    int myCatalyser;
    int oppCatalyser;
};

#endif // GAMESIMULATOR_H
