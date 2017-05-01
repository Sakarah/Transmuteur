#ifndef ACTION_H
#define ACTION_H

#include "prologin.hh"
class GameSimulator;

/// Représente une action ingame générique pour l'un des joueurs.
class Action
{
public:
    virtual ~Action() = default;

    /// Simule l'action sur le GameSimulator
    virtual void simulate(GameSimulator&) = 0;

    /// Annule la simulation sur le GameSimulator
    virtual void undo(GameSimulator&) = 0;

    /// Exécute l'action pour de vrai pour le joueur courant
    virtual void execute() = 0;

    /// Donne une réaction "intelligente" face à l'action actuelle
    virtual Action* defenseReaction(GameSimulator&) { return nullptr; }

    virtual void debugPrint() = 0;
};

#endif // ACTION_H
