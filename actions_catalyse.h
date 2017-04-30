#ifndef CATALYSE_H
#define CATALYSE_H

#include "actions_action.h"

class Catalyse : public Action
{
public:
    Catalyse(bool fromMe, bool toMe, position pos, case_type dest_type);
    void simulate(GameSimulator&) override;
    void undo(GameSimulator&) override;
    void execute() override;
    void debugPrint() override;
private:
    bool _fromMe;
    bool _toMe;
    case_type _type;
    case_type _prevType;
    position _pos;
};

#endif // CATALYSE_H
