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

    int gamePotential(bool me) const
    {
        int score = myBoard.boardPotential() + myScore + myCatalyser;
        score -= oppBoard.boardPotential() + oppScore + oppCatalyser;
        return me ? score : -score;
    }

    BoardSimulator myBoard;
    BoardSimulator oppBoard;

    int myScore;
    int oppScore;

    int myCatalyser;
    int oppCatalyser;
};

#endif // GAMESIMULATOR_H
