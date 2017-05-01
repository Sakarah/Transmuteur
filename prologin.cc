#include "prologin.hh"
#include "gamesimulator.h"
#include "prototypes.h"
#include "actions_wipeout.h"
#include <chrono>
#include <map>

/*
 * Transmuteur est une IA pour le jeu Tabula Prologinia.
 * Elle se base sur une heuristique que toute simulation d'action en tant que
 * l'IA elle-même tente de maximiser et que toute simulation d'action en
 * tant que l'adversaire tente de minimiser.
 * A chaque tour on commence par simuler son propre tour puis on choisit selon
 * le même algorithme des actions pour le tour de l'adversaire.
 * On essaye ensuite de lui donner le pire échantillon s'il jouait comme nous.
 * De plus, on regarde ce qu'il aurait fait pour pouvoir anticiper certaines
 * destructions et récupérer les points avant.
 *
 * L'IA possède de plus une détection de cycles pour éviter de rester bloquée à
 * toujours faire la même chose.
 */

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

    // Gestion des cycles
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

    // Choix des actions pour ce tour
    TurnActions turnActions = chooseBestActions(true, game, echantillon_tour());
    for(Action* a : turnActions.actionList)
    {
        a->execute();
        delete a;
    }

    erreur err = donner_echantillon(turnActions.echantillonAdv);
    if(err) printf("FAIL GiveSample (%d)", err);

    // Gestion de la fin de partie
    if(tour_actuel() >= NB_TOURS-1)
    {
        Wipeout wipe(true);
        wipe.execute();
        // TODO : gérer les derniers catalyseurs
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

