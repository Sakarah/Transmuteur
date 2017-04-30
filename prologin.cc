#include "prologin.hh"
#include "gamesimulator.h"
#include "prototypes.h"
#include "actions_wipeout.h"
#include <chrono>
#include <map>

static std::map<long long, int> cycleMap;

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
    long long gameHash = game.myBoard.hash();
    if(cycleMap.count(gameHash))
    {
        printf("Cycle detected : ");
        if(game.myScore - game.oppScore - cycleMap[gameHash] < 0) // Cycle négatif
        {
            printf("Losing cycle\n");

            int caseNumber = (tour_actuel()*7/2)%36; // 7 premier avec 36
            position pos = position{caseNumber/6,caseNumber%6};
            transmuter(pos);
            game.myBoard = BoardSimulator(moi());
            game.myCatalyser = nombre_catalyseurs();
            gameHash = game.myBoard.hash();
        }
        else printf("Winning cycle\n");
    }
    cycleMap[gameHash] = game.myScore - game.oppScore;

    TurnActions turnActions = chooseBestActions(true, game, echantillon_tour());

    for(Action* a : turnActions.actionList)
    {
        a->execute();
        delete a;
    }

    erreur err = donner_echantillon(turnActions.echantillonAdv);
    if(err) printf("FAIL GiveSample (%d)", err);

    if(tour_actuel() >= NB_TOURS-1)
    {
        Wipeout wipe(true);
        wipe.execute();
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

