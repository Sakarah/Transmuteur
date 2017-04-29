#ifndef TRANSMUTE_H
#define TRANSMUTE_H

#include "actions_action.h"

class Transmute : public Action
{
public:
    Transmute(bool me, std::vector<position> region);
    void simulate(GameSimulator&) override;
    void undo(GameSimulator&) override;
    void execute() override;
private:
    bool _me;
    case_type _type;
    std::vector<position> _region;
    int _diffScore;
    int _diffCatalyser;
};

#endif // TRANSMUTE_H
