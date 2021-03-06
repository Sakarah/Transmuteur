#ifndef TRANSMUTE_H
#define TRANSMUTE_H

#include "action.h"

/// Implémente une action de transmutation
class Transmute : public Action
{
public:
    Transmute(bool me, std::vector<position> region);
    void simulate(GameSimulator&) override;
    void undo(GameSimulator&) override;
    void execute() override;
    void debugPrint() override;
private:
    bool _me;
    case_type _type;
    std::vector<position> _region;
    int _diffScore;
    int _diffCatalyser;
};

#endif // TRANSMUTE_H
