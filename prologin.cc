#include "prologin.hh"
#include "gamesimulator.h"
#include "prototypes.h"
#include <chrono>

/// Fonction appelée au début de la partie.
void partie_init()
{
    // Rien à initialiser
}

/// Fonction appelée à chaque tour.
void jouer_tour()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();


    GameSimulator game;
    TurnActions turnActions = chooseBestActions(true, game, echantillon_tour());

    for(Action* a : turnActions.actionList)
    {
        a->execute();
        delete a;
    }

    donner_echantillon(turnActions.echantillonAdv);

    if(tour_actuel() >= NB_TOURS-1)
    {
        BoardSimulator endBoard(moi());
        for(std::vector<position> reg : endBoard.getRegions())
        {
            if(reg.size() < 2) continue;
            if(transmuter(reg[0]) != OK) printf("FAIL Transmutation finale...");
        }
    }


    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    printf("Temps: %f s\n", elapsed_seconds.count());
}

/// Fonction appelée à la fin de la partie.
void partie_fin()
{
    // Rien à libérer
}

