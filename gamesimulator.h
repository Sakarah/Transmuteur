#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "boardsimulator.h"

struct GameSimulator
{
    GameSimulator() : myBoard(moi()), oppBoard(adversaire())
    {
        myScore = score(moi());
        oppScore = score(adversaire());
        myCatalyser = 0;
        oppCatalyser = 0;
        penalty = 0;
    }

    int gamePotential(bool me) const
    {
        int score = myBoard.boardPotential() + myScore + myCatalyser;
        score -= oppBoard.boardPotential() + oppScore + oppCatalyser;
        score -= penalty;
        return me ? score : -score;
    }

    BoardSimulator myBoard;
    BoardSimulator oppBoard;

    int penalty;

    int myScore;
    int oppScore;

    int myCatalyser;
    int oppCatalyser;
};

#endif // GAMESIMULATOR_H
