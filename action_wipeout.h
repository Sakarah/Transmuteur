#ifndef WIPEOUT_H
#define WIPEOUT_H

#include "action.h"
#include "gamesimulator.h"

/// Implémente une action de wipeout intelligent qui récupère tout les points
/// possibles avant d'effacer la grille.
class Wipeout : public Action
{
public:
    Wipeout(bool me);
    void simulate(GameSimulator&) override;
    void undo(GameSimulator&) override;
    void execute() override;
    void debugPrint() override;
private:
    bool _me;
    BoardSimulator _backupBoard;
    int _scoreDiff;
    int _catalyserDiff;
};

#endif // WIPEOUT_H
