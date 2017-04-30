#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "boardsimulator.h"
#include "prototypes.h"

struct GameSimulator
{
    GameSimulator() : myBoard(moi()), oppBoard(adversaire())
    {
        myScore = score(moi());
        oppScore = score(adversaire());
        myCatalyser = nombre_catalyseurs();
        oppCatalyser = 0;
        penalty = 0;
    }

    int gamePotential(bool me, bool withCatalyser) const
    {
        std::pair<int,int> myBoardPotential = myBoard.boardPotential();
        std::pair<int,int> oppBoardPotential = oppBoard.boardPotential();
        int catalyserVal = catalyserValue();
        int onBoardCatalyserVal = (CATALYSER_BASE_VAL + catalyserVal) / 2;

        int score = myBoardPotential.first + myScore;
        score += onBoardCatalyserVal * myBoardPotential.second;
        score -= oppBoardPotential.first + oppScore;
        score -= onBoardCatalyserVal * oppBoardPotential.second;
        score -= penalty;
        if(withCatalyser) score += catalyserVal * (myCatalyser - oppCatalyser);

        return me ? score : -score;
    }

    int catalyserValue() const
    {
        std::vector<position>::size_type maxArea = 0;
        for(std::vector<position> reg : myBoard.getRegions()) maxArea = std::max(maxArea, reg.size());
        for(std::vector<position> reg : oppBoard.getRegions()) maxArea = std::max(maxArea, reg.size());
        return quantite_transmutation_or(maxArea+1) - quantite_transmutation_or(maxArea);
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
