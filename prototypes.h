#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "prologin.hh"
#include "actions_action.h"

const int INFINITY = 1000000000;
const int MIN_MAX_DEPTH = 1;
const position DIFF_POS[4] = { position{1,0}, position{0,1}, position{-1,0}, position{0,-1} };

// On pénalise les wipeout et les transmutations sur une case sinon l'algorithme n'avance jamais
const int WIPEOUT_PENALTY = 5;
const int ONE_PENALTY = 5;
const int CATALYSER_BASE_VAL = 8;

bool isValid(position pos);
bool isSide(position pos);

int regionGoldValue(int size, case_type type);
int regionCatalyserValue(int size, case_type type);

position operator+(position p1, position p2);
position operator-(position p1, position p2);

std::vector<echantillon> nextPossibleSamples(echantillon ech);

struct TurnActions
{
    std::vector<Action*> actionList;
    echantillon echantillonAdv;
    int gamePotential;
};

std::pair<Action*, int> chooseBestPlaceSample(bool me, GameSimulator& game, echantillon ech);
TurnActions chooseBestActions(bool me, GameSimulator& game, echantillon ech);

#endif // PROTOTYPES_H
