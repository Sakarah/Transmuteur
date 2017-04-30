#include "prologin.hh"
#include "gamesimulator.h"
#include "actions_placesample.h"
#include "actions_transmute.h"
#include "actions_wipeout.h"
#include "prototypes.h"

// Donne l'ownership de l'action au récepteur
std::pair<Action*, int> chooseBestPlaceSample(bool me, GameSimulator& game, echantillon ech)
{
    BoardSimulator& board = me ? game.myBoard : game.oppBoard;

    Action* bestAction = new Wipeout(me);
    bestAction->simulate(game);
    int maxBoard = game.gamePotential(me);
    bestAction->undo(game);
    for(position_echantillon pos : board.possibleSamplePos(ech))
    {
        PlaceSample* action = new PlaceSample(me, pos.pos1, pos.pos2, ech);
        action->simulate(game);
        int potential = game.gamePotential(me);
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

// Donne l'ownership des actions au récepteur (on garantit que ce sont des pointeurs valides)
TurnActions chooseBestActions(bool me, GameSimulator& game, echantillon ech)
{
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

    if(!me) return thisTurnActions;

    for(Action* a : thisTurnActions.actionList) a->simulate(game);

    int worstEchValue = -1000000000;
    for(echantillon advEch : nextPossibleSamples(ech))
    {
        TurnActions oppTurnActions = chooseBestActions(!me, game, advEch);
        for(Action* a : oppTurnActions.actionList) delete a;
        if(oppTurnActions.gamePotential > worstEchValue)
        {
            worstEchValue = oppTurnActions.gamePotential;
            thisTurnActions.echantillonAdv = advEch;
        }
    }

    return thisTurnActions;
}
