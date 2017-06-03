#ifndef ACTION_H
#define ACTION_H

#include "prologin.hh"
class GameSimulator;

/// Représente une action ingame.
class Action
{
public:
    virtual ~Action() = default;
    virtual void simulate(GameSimulator&) = 0;
    virtual void undo(GameSimulator&) = 0;
    virtual void execute() = 0;
    virtual void debugPrint() = 0;
    virtual Action* defenseReaction(GameSimulator&) { return nullptr; }
};

#endif // ACTION_H
