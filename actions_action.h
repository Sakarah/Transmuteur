#ifndef ACTION_H
#define ACTION_H

#include "gamesimulator.h"

/// Représente une action ingame.
class Action
{
public:
    virtual ~Action() = default;
    virtual void simulate(GameSimulator&) = 0;
    virtual void undo(GameSimulator&) = 0;
    virtual void execute() = 0;
    virtual void debugPrint() = 0;
};

#endif // ACTION_H
