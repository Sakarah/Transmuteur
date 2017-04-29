#include "prologin.hh"
#include "gamesimulator.h"
#include "prototypes.h"

/// Fonction appelée au début de la partie.
void partie_init()
{
    // Rien à initialiser
}

/// Fonction appelée à chaque tour.
void jouer_tour()
{
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
}

/// Fonction appelée à la fin de la partie.
void partie_fin()
{
    // Rien à libérer
}

