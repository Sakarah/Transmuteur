#include "prologin.hh"
#include "gamesimulator.h"
#include "actions_placesample.h"
#include "actions_transmute.h"
#include "actions_wipeout.h"
#include "actions_catalyse.h"
#include "prototypes.h"

/// Cherche la meilleure action de placement de l'échantillon ech
/// (ou de wipeout) à partir de la situation game.
// Ownership : Donne l'ownership de l'action au récepteur
std::pair<Action*, int> chooseBestPlaceSample(bool me, GameSimulator& game, echantillon ech)
{
    BoardSimulator& board = me ? game.myBoard : game.oppBoard;

    Action* bestAction = new Wipeout(me);
    bestAction->simulate(game);
    int maxBoard = game.gamePotential(me, true);
    bestAction->undo(game);

    for(position_echantillon pos : board.possibleSamplePos(ech))
    {
        PlaceSample* action = new PlaceSample(me, pos.pos1, pos.pos2, ech);
        action->simulate(game);
        int potential = game.gamePotential(me, true);
        action->undo(game);

        if(potential > maxBoard)
        {
            delete bestAction;
            bestAction = action;
            maxBoard = potential;
        }
        else delete action;
    }

    return std::make_pair(bestAction, maxBoard);
}

/// Renvoie la meilleure suite d'action trouvée pour un des joueur.
/// Cette fonction est la fonction principale de l'IA.
/// Elle procède par une sorte de min-max en plusieurs étapes.
// Ownership : Donne l'ownership des actions au récepteur
// (on garantit que ce sont des pointeurs valides)
TurnActions chooseBestActions(bool me, GameSimulator& game, echantillon ech)
{
    // On choisit entre enlever une région avant de jouer ou tout laisser en place
    Action* bestTransmuteAction = nullptr;
    std::pair<Action*, int> bestPlaceSampleAction = chooseBestPlaceSample(me, game, ech);

    BoardSimulator& board = me ? game.myBoard : game.oppBoard;
    for(std::vector<position> region : board.getRegions())
    {
        Transmute* transmuteAction = new Transmute(me, region);
        transmuteAction->simulate(game);
        std::pair<Action*, int> placeSampleAction = chooseBestPlaceSample(me, game, ech);
        transmuteAction->undo(game);

        if(placeSampleAction.second > bestPlaceSampleAction.second)
        {
            delete bestTransmuteAction;
            delete bestPlaceSampleAction.first;
            bestTransmuteAction = transmuteAction;
            bestPlaceSampleAction = placeSampleAction;
        }
        else
        {
            delete transmuteAction;
            delete placeSampleAction.first;
        }
    }

    TurnActions thisTurnActions;
    if(bestTransmuteAction) thisTurnActions.actionList.push_back(bestTransmuteAction);
    thisTurnActions.actionList.push_back(bestPlaceSampleAction.first);
    thisTurnActions.gamePotential = bestPlaceSampleAction.second;

    // On simule la meilleure combinaison trouvée
    for(Action* a : thisTurnActions.actionList) a->simulate(game);

    // On utilise les catalyseurs ici -> Stratégie gloutonne
    // TODO : Améliorer la stratégie d'utilisation des catalyseurs lorsque l'on en a plus qu'un.
    int& nbCatalyser = me ? game.myCatalyser : game.oppCatalyser;
    while(nbCatalyser > 0)
    {
        Action* bestCatalyse = nullptr;
        int bestCatalyseVal = game.gamePotential(me, false);
        for(int player = 1 ; player >= 0 ; player--)
        {
            BoardSimulator& board = player ? game.myBoard : game.oppBoard;
            for(int x = 0 ; x < TAILLE_ETABLI ; x++)
            {
                for(int y = 0 ; y < TAILLE_ETABLI ; y++)
                {
                    if(board.typeCase(position{x,y}) == VIDE) continue;
                    for(int t = 1 ; t < NB_TYPE_CASES ; t++)
                    {
                        Catalyse* catalyse = new Catalyse(me, player, position{x,y}, static_cast<case_type>(t));
                        catalyse->simulate(game);
                        int score = game.gamePotential(me, false);
                        catalyse->undo(game);

                        if(score > bestCatalyseVal)
                        {
                            delete bestCatalyse;
                            bestCatalyse = catalyse;
                            bestCatalyseVal = score;
                        }
                        else delete catalyse;
                    }
                }
            }
        }

        if(!bestCatalyse)
        {
            printf("Surplus de catalyseurs...\n");
            break;
        }
        bestCatalyse->simulate(game);
        thisTurnActions.actionList.push_back(bestCatalyse);
    }

    if(!me)
    {
        // On backtrack si on simule un adversaire puisqu'on a pas besoin de
        // lui faire choisir un échantillon.
        for(int i = thisTurnActions.actionList.size()-1 ; i >= 0 ; i--) thisTurnActions.actionList[i]->undo(game);
        return thisTurnActions;
    }

    // On cherche à donner la pire pièce à l'adversaire en simulant ses actions
    TurnActions worstAdvTurnActions;
    worstAdvTurnActions.gamePotential = INFINITY;
    for(echantillon advEch : nextPossibleSamples(ech))
    {
        TurnActions oppTurnActions = chooseBestActions(!me, game, advEch);
        if(oppTurnActions.gamePotential < worstAdvTurnActions.gamePotential)
        {
            for(Action* a : worstAdvTurnActions.actionList) delete a;
            worstAdvTurnActions = oppTurnActions;
            thisTurnActions.echantillonAdv = advEch;
        }
        else
        {
            for(Action* a : oppTurnActions.actionList) delete a;
        }
    }

    // On se défend d'une attaque contre nous en récoltant les points qu'on estime
    // pertinent pour l'adversaire de récupérer
    for(Action* a : worstAdvTurnActions.actionList)
    {
        Action* reaction = a->defenseReaction(game);
        if(reaction) thisTurnActions.actionList.push_back(reaction);
        delete a;
    }

    return thisTurnActions;
}
