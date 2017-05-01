#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "boardsimulator.h"
#include "prototypes.h"

/// Simulateur du jeu complet comportant les scores et les deux plateaux.
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

    /// Le potentiel de jeu dépend du potentiel des deux plateaux et des différences de score.
    int gamePotential(bool me, bool withCatalyser) const
    {
        std::pair<int,int> myBoardPotential = myBoard.boardPotential();
        std::pair<int,int> oppBoardPotential = oppBoard.boardPotential();
        int catalyserVal = catalyserValue();

        // On modélise la valeur d'un catalyseur sur un plateau comme la moyenne entre
        // une valeur à l'instant t et une base indicatrice d'une bonne utilisation d'un catalyseur
        // à n'importe quel moment.
        int onBoardCatalyserVal = (CATALYSER_BASE_VAL + catalyserVal) / 2;

        int score = myBoardPotential.first + myScore;
        score += onBoardCatalyserVal * myBoardPotential.second;
        score -= oppBoardPotential.first + oppScore;
        score -= onBoardCatalyserVal * oppBoardPotential.second;
        score -= penalty;
        if(withCatalyser) score += catalyserVal * (myCatalyser - oppCatalyser);

        return me ? score : -score; // L'adversaire voit le potentiel en négatif par rapport à nous
    }

    /// Les catalyseurs n'ont pas toujours la même valeur au cours de la partie.
    /// Pour celà on modélise leur valeur en fonction de la taille de la plus grande zone,
    /// tout plateau confondu.
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
